@echo off
setlocal EnableDelayedExpansion

REM ============================================================
REM AlgoSketch build script (Windows / MinGW-w64)
REM Requires: gcc (MinGW-w64) and the bundled win_flex/win_bison
REM ============================================================

set ROOT=%~dp0
set SRC=%ROOT%src
set TOOLS=%ROOT%tools
set BISON=%TOOLS%\win_bison.exe
set FLEX=%TOOLS%\win_flex.exe
set GCC=gcc
set EXE=%ROOT%algosketch.exe

echo [AlgoSketch] Generating parser with win_bison...
pushd "%SRC%"
"%BISON%" -d -o parser.c parser.y
if errorlevel 1 ( popd & goto error )

echo [AlgoSketch] Generating lexer with win_flex...
"%FLEX%" -o lexer.c lexer.l
if errorlevel 1 ( popd & goto error )
popd

echo [AlgoSketch] Compiling AlgoSketch compiler...
"%GCC%" -O2 -Wall -o "%EXE%" ^
    "%SRC%\main.c" ^
    "%SRC%\parser.c" "%SRC%\lexer.c" ^
    "%SRC%\ast.c" ^
    "%SRC%\error.c" ^
    "%SRC%\symbol_table.c" ^
    "%SRC%\semantic.c" ^
    "%SRC%\tac.c" ^
    "%SRC%\codegen.c" ^
    -I"%SRC%"
if errorlevel 1 goto error

echo [AlgoSketch] Build successful: %EXE%
exit /b 0

:error
echo [AlgoSketch] Build failed.
exit /b 1
