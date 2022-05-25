:: Run all Google Test C++ tests registered with CMake.
::
:: All arguments passed to this script are passed to ctest.
::
:: If Google Test is not installed in a standard system location, you need to
:: set GTEST_ROOT or the test runner will fail to start.

@echo off
setlocal EnableDelayedExpansion

:: top-level build dir
set BUILD_WINDOWS_DIR=%~dp0\build_windows
:: names of the Google Test .lib files with main
set GTEST_LIBS=gtest_maind.lib gtest_main.lib

call :Main %*
exit /b %ERRORLEVEL%

::::
:: Find gtest_main[d].lib installation and ensure it is correct build.
::
:: If GTEST_ROOT is defined, %GTEST_ROOT%\lib is added to PATH.
::
:: Relies on the CMakeCache.txt existing in the build_windows directory to
:: determine whether the Debug or Release gtest_main should be used.
::
:FindGTest
:: if GTEST_ROOT not defined, don't update PATH
if not defined GTEST_ROOT (
    echo Note: GTEST_ROOT not set
) else (
    set PATH=%GTEST_ROOT%\lib;!PATH!
)
:: for Debug, Release builds, try to find the right gtest_main[d].lib. success
:: determined by defining GTEST_MAIN_COMPAT_FOUND; on success, echo location
findstr /b CMAKE_BUILD_TYPE:STRING=Debug %BUILD_WINDOWS_DIR%\CMakeCache.txt ^
    >nul 2>&1
if %ERRORLEVEL% equ 0 (
    where gtest_maind.lib >nul 2>&1
    if !ERRORLEVEL! gtr 0 (
        echo Error: Could not find gtest_maind.lib, please update GTEST_ROOT
        exit /b 1
    )
    for /f %%i in ('where gtest_maind.lib') do set GTEST_MAIN_LOCATION=%%i
)
findstr /b CMAKE_BUILD_TYPE:STRING=Release %BUILD_WINDOWS_DIR%\CMakeCache.txt ^
    >nul 2>&1
if %ERRORLEVEL% equ 0 (
    where gtest_main.lib >nul 2>&1
    if !ERRORLEVEL! gtr 0 (
        echo Error: Could not find gtest_main.lib, please update GTEST_ROOT
        exit /b 1
    )
    for /f %%i in ('where gtest_main.lib') do set GTEST_MAIN_LOCATION=%%i
)
:: echo path and return success if located compatible build. however, if second
:: findstr command also fails, i.e. GTEST_MAIN_LOCATION is not defined, then
:: there is no CMakeCache.txt. warn, and try to find the Google Test main libs
:: ignoring build type, but allow test running to proceed (might fail).
if defined GTEST_MAIN_LOCATION (
    echo Found compatible Google Test LIB %GTEST_MAIN_LOCATION%
    exit /b 0
) else (
    echo Warning: %BUILD_WINDOWS_DIR%\CMakeCache.txt missing
    echo Looking for any version of gtest_main[d].lib...
    for %%n in (%GTEST_LIBS%) do (
        where %%n >nul 2>&1
        if !ERRORLEVEL! equ 0 (
            for /f %%i in ('where %%n') do (
                set GTEST_MAIN_LOCATION=%%i
            )
            echo Found [link compatibility unknown] !GTEST_MAIN_LOCATION!
        )
    )
    exit /b 0
)
:: if no Google Test lib with main can be found, can't run tests
exit /b 1

::::
:: Main function executing the test runner.
::
:Main
:: if we can't find Google Test static lib with main, just exit
call :FindGTest
if %ERRORLEVEL% gtr 0 (
    exit /b 1
)
:: need to put pdcip_c DLL on path + ensure that Google Test has color output
set PATH=%BUILD_WINDOWS_DIR%\src;%PATH%
set GTEST_COLOR=yes
ctest --test-dir %BUILD_WINDOWS_DIR% %*
exit /b 0

endlocal
echo on
