@echo off
REM Build TypeLib on Windows NT and Chicago systems
REM %1 = file name, including extension
REM %2 = bin path, e.g. c:\msvc\bin
REM %3 = file/project path
REM %4 = include path (opt), e.g. /Ic:\msvc\cdk16\include

cd %3
if not exist tlb16\nul md tlb16
if exist tlb16\%1 del tlb16\%1
%2\cl /c /D__MKTYPLIB__ %4 /E %1 >tlb16\%1
cd tlb16
%2\mktyplib /W0 /nocpp /nologo %4 %1
