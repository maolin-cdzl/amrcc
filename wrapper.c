/* ------------------------------------------------------------------
 * Copyright (C) 2009 Martin Storsjo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */

#define AMRNB_WRAPPER_INTERNAL
#include "sp_dec.h"
#include "amrdecode.h"
#include "amrencode.h"
#include "amrcc/interf_dec.h"
#include "amrcc/interf_enc.h"
#include <stdlib.h>

#ifndef DISABlE_SPEEX_PREPROCESS
#include "speex/speex_preprocess.h"
#endif


#define PCM_SAMPLES_PER_FRAME		160
#define MAX_PACKED_SIZE (MAX_SERIAL_SIZE / 8 + 2)
#define SERIAL_FRAMESIZE (1+MAX_SERIAL_SIZE+5)

#ifdef __cplusplus
extern "C" {
#endif

const static int s_frame_size[16] = {13, 14, 16, 18, 20, 21, 27, 32, 6, 0, 0, 0, 0, 0, 0, 1};

#ifndef DISABLE_AMRNB_DECODER
DLL_PUBLIC void* DecoderAmrInit(void) {
	void* ptr = NULL;
	GSMInitDecode(&ptr, (int8*)"Decoder");
	return ptr;
}

DLL_PUBLIC void DecoderAmrExit(void* state) {
	GSMDecodeFrameExit(&state);
}

DLL_PUBLIC void DecoderAmrDecodeFrame(void* state, const unsigned char* in, short* out, int bfi) {
	unsigned char type = (in[0] >> 3) & 0x0f;
	in++;
	AMRDecode(state, (enum Frame_Type_3GPP) type, (UWord8*) in, out, MIME_IETF);
}

DLL_PUBLIC int DecoderAmrDecode(void* c, const unsigned char* amr,const int amr_bytes, short* pcm, int bfi) {
	if( c == 0 || amr == 0 || pcm == 0 || amr_bytes <= 0 )
		return 0;

	int proc_bytes = 0;
	int output_samples = 0;
	UWord8 ft;
	while( proc_bytes < amr_bytes ) {
		ft = (((amr+proc_bytes)[0]) >> 3) & 0x0F;
		if( s_frame_size[ft] == 0 || proc_bytes + s_frame_size[ ft ] > amr_bytes )
			break;
		
		/* decode frame */
		DecoderAmrDecodeFrame(c,amr+proc_bytes, pcm+output_samples,bfi);
		proc_bytes += s_frame_size[ ft ];
		output_samples += PCM_SAMPLES_PER_FRAME;
	}
	return output_samples;
}

#endif

#ifndef DISABLE_AMRNB_ENCODER
struct encoder_state {
	void* encCtx;
	void* pidSyncCtx;
#ifndef DISABlE_SPEEX_PREPROCESS
	SpeexPreprocessState *		m_spx_input_pre_st;
#endif
};

DLL_PUBLIC void* EncoderAmrInit(int dtx) {
	struct encoder_state* state = (struct encoder_state*) malloc(sizeof(struct encoder_state));
	AMREncodeInit(&state->encCtx, &state->pidSyncCtx, dtx);

#ifndef DISABlE_SPEEX_PREPROCESS
	state->m_spx_input_pre_st = speex_preprocess_state_init(160,8000);

	int enabled = 1;
	enabled = 1;
	speex_preprocess_ctl(state->m_spx_input_pre_st ,SPEEX_PREPROCESS_SET_VAD, &enabled);
	enabled = 0;
	speex_preprocess_ctl(state->m_spx_input_pre_st ,SPEEX_PREPROCESS_SET_DEREVERB , &enabled);
	enabled = 0;
	speex_preprocess_ctl(state->m_spx_input_pre_st ,SPEEX_PREPROCESS_SET_ECHO_STATE , NULL);

	enabled = 1;
	speex_preprocess_ctl(state->m_spx_input_pre_st , SPEEX_PREPROCESS_SET_DENOISE , &enabled);
	int noise_suppress = -24;
	speex_preprocess_ctl(state->m_spx_input_pre_st , SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noise_suppress);


	enabled = 1;
	speex_preprocess_ctl(state->m_spx_input_pre_st , SPEEX_PREPROCESS_SET_AGC, &enabled);
	float level = 24000.0;
	speex_preprocess_ctl(state->m_spx_input_pre_st, SPEEX_PREPROCESS_SET_AGC_LEVEL, &level);

#endif
	return state;
}

DLL_PUBLIC void EncoderAmrExit(void* s) {
	struct encoder_state* state = (struct encoder_state*) s;
	AMREncodeExit(&state->encCtx, &state->pidSyncCtx);
#ifndef DISABlE_SPEEX_PREPROCESS
	speex_preprocess_state_destroy(state->m_spx_input_pre_st);
#endif
	free(state);
}

static int EncoderAmrEncodeFrame(void* s, enum Mode mode, const short* speech, unsigned char* out, int forceSpeech) {
	struct encoder_state* state = (struct encoder_state*) s;
#ifndef DISABlE_SPEEX_PREPROCESS
	speex_preprocess_run(state->m_spx_input_pre_st,speech);
#endif
	enum Frame_Type_3GPP frame_type = (enum Frame_Type_3GPP) mode;
	int ret = AMREncode(state->encCtx, state->pidSyncCtx, mode, (Word16*) speech, out, &frame_type, AMR_TX_IETF);
	out[0] |= 0x04;
	return ret;
}

DLL_PUBLIC int EncoderAmrEncode(void* c,enum Mode mode,const short* pcm,const int input_samples,unsigned char* amr, int force_speech) {
	if( c == 0 || pcm == 0 || amr == 0 )
		return 0;

	int proc_samples = 0;
	int output_bytes = 0;
	int frame_bytes = 0;
	while( proc_samples <= input_samples - PCM_SAMPLES_PER_FRAME ) {
		frame_bytes = EncoderAmrEncodeFrame(c,mode,pcm + proc_samples,amr + output_bytes,force_speech);
		output_bytes += frame_bytes;
		proc_samples += PCM_SAMPLES_PER_FRAME;
	}
	return output_bytes;
}
#endif


DLL_PUBLIC int EncoderAmrMaxSize(int pcm_samples,enum Mode mode) {
	int frame_count = pcm_samples / PCM_SAMPLES_PER_FRAME;
	return frame_count * s_frame_size[mode];
}

DLL_PUBLIC int DecoderAmrGetFrameSize(unsigned char toc) {
	return s_frame_size[ ((toc) >> 3) & 0x0F ];
}

DLL_PUBLIC int DecoderAmrIsSilenceFrame(unsigned char toc) {
	enum Frame_Type_3GPP type = (enum Frame_Type_3GPP) ( ((toc) >> 3) & 0x0F );
	return type > AMR_122;
}

DLL_PUBLIC int DecoderAmrGetSamples(const unsigned char* amr,const int amr_bytes) {
	int proc_bytes = 0;
	int output_samples = 0;
	unsigned char toc = 0;
	unsigned char ft;
	while( proc_bytes < amr_bytes ) {
		toc = (amr+proc_bytes)[0];
		ft = (toc >> 3) & 0x0F;
		
		if( s_frame_size[ft] == 0 || proc_bytes + s_frame_size[ ft ] > amr_bytes )
			break;
		proc_bytes += s_frame_size[ft];
		output_samples += PCM_SAMPLES_PER_FRAME;
	}
	return output_samples;
}



#ifdef __cplusplus
}
#endif

