:: Run all Check C tests registered with CMake.
::
:: All arguments passed to this script are passed to ctest.
::
:: If Check is not installed in a standard system location, you need to set
:: CHECK_ROOT or the test runner will fail to start.

@echo off
setlocal EnableDelayedExpansion

:: top-level build dir
set BUILD_WINDOWS_DIR=%~dp0\build_windows

call :Main %*
exit /b %ERRORLEVEL%

::::
:: Find checkShared.dll installation by checking CHECK_ROOT and PATH.
::
:: If CHECK_ROOT is defined, %CHECK_ROOT%\bin is added to PATH.
::
:FindCheck
:: if CHECK_ROOT not defined, don't update PATH
if not defined CHECK_ROOT (
    echo Note: CHECK_ROOT not set
) else (
    set PATH=%CHECK_ROOT%\bin;!PATH!
)
:: try to find checkDynamic.dll using path, error if we can't find it
where checkDynamic.dll >nul 2>&1
if %ERRORLEVEL% gtr 0 (
    echo Error: Could not find checkDynamic.dll, please update CHECK_ROOT
    exit /b 1
)
:: set location of checkDynamic.dll and echo success
for /f %%i in ('where checkDynamic.dll') do set CHECK_SHARED_LOCATION=%%i
echo Found Check DLL %CHECK_SHARED_LOCATION%
exit /b 0

::::
:: Main function executing the test runner.
::
:: Arguments:
::  Array of command-line arguments
::
:Main
:: if we can't find checkDynamic.dll, just exit
call :FindCheck
if %ERRORLEVEL% gtr 0 (
    exit /b 1
)
:: need to put pdcip_c DLL on path too for test runner
set PATH=%BUILD_WINDOWS_DIR%\src;%PATH%
ctest --test-dir %BUILD_WINDOWS_DIR% %*
exit /b 0

endlocal
echo on
