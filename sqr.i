%module SQR
%{
#include "QR_Encode.h"
#include "win2ansi.h"
#include "draw/qdr.h"
%}
%include "QR_Encode.h"
%include "win2ansi.h"
%include "draw/qdr.h"

%typemap(in) char * {
   $1 = SvPV($input,PL_na);
}

