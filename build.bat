@echo off

SET SOURCES=../src/main.cpp  ../src/game.cpp ../src/platform.cpp ../src/renderer.cpp ../src/glad/glad.c

:: ../src/win32_platform.cpp

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

pushd .
if not exist "build" mkdir "build"
cd build
:: EXE
cl /I../src/ /std:c++20 /Zi /FS /EHsc /nologo %SOURCES% /link user32.lib gdi32.lib shell32.lib Opengl32.lib
if %ERRORLEVEL% NEQ 0 (
    echo Build failed
    goto cleanup
)

:cleanup
popd
