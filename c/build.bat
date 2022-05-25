:: Thin wrapper for ../tools/cmake_build.bat to build the C library and tests.

@echo off
setlocal EnableDelayedExpansion

%~dp0\..\tools\cmake_build.bat %~dp0 %*

endlocal
echo on
