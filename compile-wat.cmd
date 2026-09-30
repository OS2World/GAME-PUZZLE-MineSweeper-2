@echo off
rem Mine Sweeper/2 - OpenWatcom build script

set LOGFILE=compile-wat.log
if exist %LOGFILE% del %LOGFILE%

rem -- Detect OpenWatcom --
if exist c:\watcom2\binp\wcc386.exe set WATCOM=c:\watcom2
if exist c:\watcom\binp\wcc386.exe  set WATCOM=c:\watcom

if "%WATCOM%"=="" goto nowatcom

rem -- Default OS2TK --
if "%OS2TK%"=="" set OS2TK=c:\os2tk45

set PATH=%WATCOM%\binp;%WATCOM%\bin;%PATH%
set INCLUDE=%WATCOM%\h;%WATCOM%\h\os2
set LIB=%WATCOM%\lib386;%WATCOM%\lib386\os2

echo Using WATCOM=%WATCOM% | tee -a %LOGFILE%
echo Using OS2TK=%OS2TK%   | tee -a %LOGFILE%

echo. | tee -a %LOGFILE%
echo Cleaning... | tee -a %LOGFILE%
wmake -f makefile.wat clean 2>&1 | tee -a %LOGFILE%

echo. | tee -a %LOGFILE%
echo Building... | tee -a %LOGFILE%
wmake -f makefile.wat all 2>&1 | tee -a %LOGFILE%

if exist bin\mine.exe goto buildok

echo. | tee -a %LOGFILE%
echo BUILD FAILED | tee -a %LOGFILE%
goto end

:buildok
echo. | tee -a %LOGFILE%
echo BUILD OK | tee -a %LOGFILE%
goto end

:nowatcom
echo ERROR: OpenWatcom not found at c:\watcom or c:\watcom2
echo ERROR: OpenWatcom not found >> %LOGFILE%

:end
