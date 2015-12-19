SET OF_ROOT=/F/of/openframeworks/msys
SET PATH=F:\OF\msys2\mingw32\bin;F:\OF\msys2\usr\bin;F:\windows\system32;F:\OF\msys2\mingw32\i686-w64-mingw32\bin;
make Release PLATFORM_OS=MINGW32_NT && if ERRORLEVEL 0 GOTO Deploy else GOTO End

:End
pause
GOTO:EOF

:Deploy
if not exist bin\data mkdir bin\data
copy data bin\data /y
copy msys_libs\**.* bin /y

pause