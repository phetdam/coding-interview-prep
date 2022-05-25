:: CMake wrapper script to build the CMake targets in a specified subdirectory.
:: Any arguments before --build-args are passed directly to cmake --build.
::
:: Note: any -D<var>=<value> arguments must be double-quoted!

@echo off
setlocal EnableDelayedExpansion

:: arguments passed to cmake command directly
set CMAKE_ARGS=
:: arguments passed to cmake --build command directly
set CMAKE_BUILD_ARGS=
:: number of args. we have to compute this manually, no $# like in [ba]sh
set /a N_ARGS=0
:: program name, as calling labels sets %0
set PROGNAME=%0

call :Main %*
exit /b %ERRORLEVEL%

::::
:: Print usage on error or when requested.
::
:DisplayUsage
:: can't use %0, as call make %0 :DisplayUsage
echo Usage: %PROGNAME% src_dir [-h] [args ...] [--build-args build_args ...]
echo.
echo CMake wrapper script to build CMake targets given a specified subdirectory.
echo.
echo Specified subdirectory corresponds to the CMAKE_SOURCE_DIR CMake variable.
echo The corresponding build directory will be src_dir/build.
echo.
echo positional arguments:
echo  src_dir  Top-level source directory with CMakeLists.txt
echo.
echo optional arguments:
echo  -h, --help      show this usage
echo  args ...        extra arguments to pass to the cmake command
echo  build_args ...  extra arguments to pass to the cmake --build command"
exit /b 0

::::
:: Set N_ARGS to determine the number of CLI arguments to the script.
::
:: Arguments:
::  Array of command-line arguments
::
:SetNArgs
for %%a in (%*) do set /a N_ARGS+=1
exit /b 0

::::
:: Main function for the build script.
::
:: Arguments:
::  Array of command-line arguments
::
:Main
:: must manually set N_ARGS to get number of arguments
call :SetNArgs %*
:: handle no args and when help is requested
if %N_ARGS%==0 (
    echo Error: Must provide src_dir. Try %0 --help for usage
    exit /b 1
)
:: no conditional operators, so we do things the sad way
if %1==-h (
    call :DisplayUsage
    exit /b 0
)
if %1==--help (
    call :DisplayUsage
    exit /b 0
)
:: absolute path (drive, path, name, [e]xtension) for source, build directories
set SRC_DIR=%~dpnx1
set BUILD_DIR=%SRC_DIR%\build_windows
:: separate incoming args into those for cmake, cmake --build. note that the
:: only way to preserve literal "=" is to just accept all the args.
call :CollectArgs %*
cmake -G Ninja -S %SRC_DIR% -B %BUILD_DIR% %CMAKE_ARGS%
:: update PATH else test runners will fail to link against pdcip_cpp.dll
set PATH=%BUILD_DIR%\src;%PATH%
cmake --build %BUILD_DIR% %CMAKE_BUILD_ARGS%
exit /b 0

::::
:: Collect incoming arguments, separating them for cmake, cmake --build.
::
:: Sets the CMAKE_ARGS and CMAKE_BUILD_ARGS global variables. %1 is skipped
:: since that is assumed to be the target source directory.
::
:: Arguments:
::  Array of command-line arguments
::      Any arguments after --build-args are passed to cmake --build while the
::      rest preceding will be passed to the cmake configure command.
::
:CollectArgs
:: define after we see the --build-args flag
set POPULATE_BUILD_ARGS=
:: defined once we have skipped %1 (source directory) in %*
set CAN_COLLECT_ARGS=
:: if CMAKE_BUILD_ARGS, CMAKE_ARGS empty, set directly, else append value to
:: the end of CMAKE_BUILD_ARGS, CMAKE_ARGS. note that using "::" as comment can
:: cause cmd to misinterpret tokens as drive names when you have "::" inside
:: the if statements. use rem instead. however, rem tends to be much slower.
::
:: see https://stackoverflow.com/a/12407934/14227825 and other answers.
::
for %%A in (%*) do (
    if %%A==--build-args (
        set POPULATE_BUILD_ARGS=1
    ) else (
        if defined POPULATE_BUILD_ARGS (
            if not defined CMAKE_BUILD_ARGS (
                set CMAKE_BUILD_ARGS=%%A
            ) else (
                set CMAKE_BUILD_ARGS=!CMAKE_BUILD_ARGS! %%A
            )
        ) else (
            if not defined CAN_COLLECT_ARGS (
                set CAN_COLLECT_ARGS=1
            ) else (
                if not defined CMAKE_ARGS (
                    set CMAKE_ARGS=%%A
                ) else (
                    set CMAKE_ARGS=!CMAKE_ARGS! %%A
                )
            )
        )
    )
)
:: unset variables (although not needed here, good practice)
set POPULATE_BUILD_ARGS=
set CAN_COLLECT_ARGS=
exit /b 0

endlocal
echo on
