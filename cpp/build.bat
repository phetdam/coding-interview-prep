:: Simple CMake wrapper script to build the CMake targets in the project.
:: Any arguments before --build-args are passed directly to cmake --build.
::
:: Note: any -D<var>=<value> arguments must be double-quoted!

@echo off
setlocal

:: arguments passed to cmake command directly
set CMAKE_ARGS=
:: arguments passed to cmake --build command directly
set CMAKE_BUILD_ARGS=

call :Main %*
exit /b %ERRORLEVEL%

::::
:: Main function for the build script.
::
:: Arguments:
::  Array of command-line arguments
::
:Main
:: separate incoming args into those for cmake, cmake --build. note that the
:: only way to preserve literal "=" is to just accept all the args.
call :CollectArgs %*
cmake -G Ninja -S . -B build_windows %CMAKE_ARGS%
cmake --build build_windows %CMAKE_BUILD_ARGS%
exit /b 0

::::
:: Collect incoming arguments, separating them for cmake, cmake --build.
::
:: Sets the CMAKE_ARGS and CMAKE_BUILD_ARGS global variables.
::
:: Arguments:
::  Array of command-line arguments
::      Any arguments after --build-args are passed to cmake --build while the
::      rest preceding will be passed to the cmake configure command.
::
:CollectArgs
:: set to 1 after we see the --build-args flag
set POPULATE_BUILD_ARGS=0
:: need parentheses if %1 is an array. if CMAKE_BUILD_ARGS, CMAKE_ARGS empty,
:: set directly, else append value to the end of CMAKE_BUILD_ARGS, CMAKE_ARGS.
:: note that using "::" as comment can cause cmd to misinterpret tokens as
:: drive names when you have "::" inside the if statements. use rem instead.
:: however, rem tends to be much slower.
::
:: see https://stackoverflow.com/a/12407934/14227825 and other answers.
::
for %%A in (%*) do (
    if %%A==--build-args (
        set POPULATE_BUILD_ARGS=1
    ) else (
        if !POPULATE_BUILD_ARGS!==1 (
            if not defined CMAKE_BUILD_ARGS (
                set CMAKE_BUILD_ARGS=%%A
            ) else (
                set CMAKE_BUILD_ARGS=!CMAKE_BUILD_ARGS! %%A
            )
        ) else (
            if not defined CMAKE_ARGS (
                set CMAKE_ARGS=%%A
            ) else (
                set CMAKE_ARGS=!CMAKE_ARGS! %%A
            )
        )
    )
)
exit /b 0

endlocal
echo on
