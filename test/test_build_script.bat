@echo off
REM Simple test script to identify the problematic line
REM Run this to see where the error occurs

echo ========================================
echo Testing Batch Script Syntax
echo ========================================
echo.

setlocal enabledelayedexpansion

echo [TEST 1] Basic variable assignment
set TEST_VAR=test
echo   TEST_VAR=%TEST_VAR%
echo   PASSED
echo.

echo [TEST 2] Parameter parsing
echo   Arg1: %~1
echo   Arg2: %~2
set ARG1=%~1
set ARG2=%~2
echo   ARG1=%ARG1%
echo   ARG2=%ARG2%
echo   PASSED
echo.

echo [TEST 3] If statement with parameter
if not "%~1"=="" (
    echo   Parameter 1 is not empty: %~1
) else (
    echo   Parameter 1 is empty
)
echo   PASSED
echo.

echo [TEST 4] If statement with variable
set TEST_VAR=
if "%TEST_VAR%"=="" (
    echo   TEST_VAR is empty
) else (
    echo   TEST_VAR is not empty
)
echo   PASSED
echo.

echo [TEST 5] Nested if statements
if "%TEST_VAR%"=="" (
    if not "%~1"=="" (
        echo   Both conditions true
    ) else (
        echo   First true, second false
    )
) else (
    echo   First condition false
)
echo   PASSED
echo.

echo [TEST 6] Error level check
where cmake >nul 2>&1
if errorlevel 1 (
    echo   CMake not found
) else (
    echo   CMake found
)
echo   PASSED
echo.

echo [TEST 7] For loop
for /f "tokens=*" %%i in ('echo test') do (
    set LOOP_VAR=%%i
    echo   Loop var: !LOOP_VAR!
)
echo   PASSED
echo.

echo ========================================
echo All tests passed!
echo ========================================
pause

