@echo off
echo Compiling C program with Visual Studio Build Tools...
echo.

REM Check for Visual Studio compiler
where cl >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: Visual Studio compiler (cl) not found!
    echo.
    echo Please install Visual Studio Build Tools:
    echo https://visualstudio.microsoft.com/visual-cpp-build-tools/
    echo.
    echo Then run this from "Developer Command Prompt for VS"
    echo.
    pause
    exit /b 1
)

REM Compile with Visual Studio
echo Compiling todo.c with cl...
cl todo.c /Fe:todo.exe
if %errorlevel% neq 0 (
    echo ERROR: Compilation failed!
    pause
    exit /b 1
)

REM Clean up Visual Studio files
del *.obj >nul 2>nul

echo.
echo SUCCESS: C program compiled successfully!
echo.
echo You can now run:
echo   todo.exe list    - List all tasks
echo   todo.exe add "task description"    - Add a new task
echo   todo.exe update id 1    - Mark task as completed
echo   todo.exe delete id    - Delete a task
echo.
pause
