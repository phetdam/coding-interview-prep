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
:: Main function executing the test runner.
::
:Main
:: if CHECK_ROOT not defined and we can't find checkDynamic DLL, warn
if not defined CHECK_ROOT (
    echo note: CHECK_ROOT not set
    where checkDynamic.dll >nul 2>&1
    if !ERRORLEVEL! gtr 0 (
        echo error: could not find checkDynamic.dll, please set CHECK_ROOT
    )
    exit /b 1
)
:: need to put pdcip_c, checkDynamic DLL on path, else Windows can't find it
set PATH=%BUILD_WINDOWS_DIR%\src;%CHECK_ROOT%\bin;%PATH%
ctest --test-dir %BUILD_WINDOWS_DIR% %*
exit /b 0

endlocal
echo on
