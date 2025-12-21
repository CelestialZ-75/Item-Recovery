# Debug Guide for build_and_run.bat

If you encounter the error `": was unexpected at this time"`, follow these debugging steps:

## Step 1: Run the Test Script

First, test if your batch script environment is working correctly:

```cmd
test_build_script.bat
```

This will test various batch script syntax patterns. If this fails, the issue is with your Windows batch interpreter.

## Step 2: Run the Debug Version

Use the debug version which shows detailed execution information:

```cmd
build_and_run_debug.bat GUI "C:\Qt\5.15.2\msvc2019_64"
```

Or with full command echo (shows every command executed):

```cmd
build_and_run_debug.bat DEBUG GUI "C:\Qt\5.15.2\msvc2019_64"
```

The debug version will:
- Show all variable values at each step
- Display which line is being executed
- Show detailed error information
- Help identify the exact line causing the error

## Step 3: Manual Testing

If the error persists, try running commands manually:

1. **Test parameter parsing:**
   ```cmd
   @echo off
   setlocal enabledelayedexpansion
   set ARG1=%~1
   set ARG2=%~2
   echo ARG1=%ARG1%
   echo ARG2=%ARG2%
   ```

2. **Test if statements:**
   ```cmd
   @echo off
   set TEST_VAR=test
   if not "%TEST_VAR%"=="" (
       echo Variable is not empty
   )
   ```

3. **Test errorlevel:**
   ```cmd
   @echo off
   where cmake >nul 2>&1
   if errorlevel 1 (
       echo CMake not found
   ) else (
       echo CMake found
   )
   ```

## Common Issues and Solutions

### Issue 1: Parameter with Special Characters

**Problem:** If your Qt path contains special characters (like parentheses), it may cause issues.

**Solution:** Always quote the path:
```cmd
build_and_run.bat GUI "C:\Program Files (x86)\Qt\5.15.2\msvc2019_64"
```

### Issue 2: Empty Parameters

**Problem:** Empty parameters can cause syntax errors.

**Solution:** The script now uses intermediate variables to handle this safely.

### Issue 3: Delayed Expansion Issues

**Problem:** Variables inside loops or parentheses may not expand correctly.

**Solution:** The script uses `enabledelayedexpansion` and `!VAR!` syntax where needed.

## Getting More Information

If you still encounter errors:

1. **Check Windows version:**
   ```cmd
   ver
   ```

2. **Check command processor:**
   ```cmd
   echo %COMSPEC%
   ```

3. **Run with echo on:**
   Add `echo on` at the top of the script (after `@echo off`) to see every command.

4. **Check for hidden characters:**
   The script file might have encoding issues. Ensure it's saved as:
   - Encoding: ANSI or Windows-1252
   - Line endings: CRLF (Windows)

## Alternative: Use CMake Directly

If the batch script continues to cause issues, you can use CMake directly:

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="C:\Qt\5.15.2\msvc2019_64"
cmake --build . --config Release
```

Then manually deploy Qt libraries and run the executable.

## Reporting Issues

If you find the problematic line, please report:
1. The exact error message
2. The line number (if visible in debug mode)
3. Your Windows version (`ver` command output)
4. The command you used to run the script

