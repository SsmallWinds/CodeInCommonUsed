%module(directors="1") testoutput

%{
#include "Test.h"
#include "Output.h"
%}


%javaconst(1);

%include "Output.h"

%feature("director") ITestCallBack2;
%include "Test.h"