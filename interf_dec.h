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

#ifndef OPENCORE_AMRNB_INTERF_DEC_H
#define OPENCORE_AMRNB_INTERF_DEC_H

#include "amr_export.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_PUBLIC void* DecoderAmrInit(void);
DLL_PUBLIC void DecoderAmrExit(void* state);
DLL_PUBLIC void DecoderAmrDecodeFrame(void* state, const unsigned char* in, short* out, int bfi);
DLL_PUBLIC int DecoderAmrDecode(void* c, const unsigned char* amr,const int amr_bytes, short* pcm, int bfi);
DLL_PUBLIC int DecoderAmrGetFrameSize(unsigned char toc);
DLL_PUBLIC int DecoderAmrGetSamples(const unsigned char* amr,const int amr_bytes);
DLL_PUBLIC int DecoderAmrIsSilenceFrame(unsigned char toc);

#ifdef __cplusplus
}
#endif

#endif
