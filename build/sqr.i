%module SQR
%{
#include "encode/QR_Encode.h"
#include "encode/win2ansi.h"
#include "draw/qdr.h"
%}
%include "encode/QR_Encode.h"
%include "encode/win2ansi.h"
%include "draw/qdr.h"

%typemap(in) const char * {
   $1 = SvPV($input,PL_na);
}

