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

#ifndef OPENCORE_AMRNB_INTERF_ENC_H
#define OPENCORE_AMRNB_INTERF_ENC_H

#include "amrcc/amr_export.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AMRNB_WRAPPER_INTERNAL
/* Copied from enc/src/gsmamr_enc.h */
enum Mode {
	MR475 = 0,/* 4.75 kbps */
	MR515,    /* 5.15 kbps */
	MR59,     /* 5.90 kbps */
	MR67,     /* 6.70 kbps */
	MR74,     /* 7.40 kbps */
	MR795,    /* 7.95 kbps */
	MR102,    /* 10.2 kbps */
	MR122,    /* 12.2 kbps */
	MRDTX,    /* DTX       */
	N_MODES   /* Not Used  */
};
#endif

DLL_PUBLIC void* EncoderAmrInit(int dtx);
DLL_PUBLIC void EncoderAmrExit(void* state);
DLL_PUBLIC int EncoderAmrEncode(void* c,enum Mode mode,const short* pcm,const int input_samples,unsigned char* amr, int force_speech);
DLL_PUBLIC int EncoderAmrMaxSize(int pcm_samples,enum Mode mode);

#ifdef __cplusplus
}
#endif

#endif
