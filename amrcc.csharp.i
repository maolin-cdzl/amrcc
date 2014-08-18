%module amrcc
%include <windows.i>
%include <csharp/typemaps.i>
%include <csharp/arrays_csharp.i>

%{
#include "typedef.h"
#include "interf_enc.h"
#include "interf_dec.h"
%}

%apply unsigned char OUTPUT[] { unsigned char* }
%apply unsigned char INPUT[] { const unsigned char* }
%apply signed short OUTPUT[] { short* }
%apply signed short INPUT[] { const short* }

%apply void *VOID_INT_PTR { void* }


%include "typedef.h"
%include "interf_enc.h"
%include "interf_dec.h"
