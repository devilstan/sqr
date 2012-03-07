%module QREncode
%{
#include "QR_Encode.h"
#include "win2ansi.h"
%}
%include "QR_Encode.h"
%include "win2ansi.h"

%typemap(in) char * {
   $1 = SvPV($input,PL_na);
}

