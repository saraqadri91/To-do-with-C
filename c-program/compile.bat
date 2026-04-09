@echo off
echo Compiling C program for To-Do List Application...
echo.

REM Check if GCC is available
where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: GCC compiler not found!
    echo.
    echo Please install GCC compiler:
    echo - Windows: Install MinGW or use WSL
    echo - Or download from: https://gcc.gnu.org/install/
    echo.
    pause
    exit /b 1
)

REM Compile the C program
echo Compiling todo.c...
gcc todo.c -o todo.exe
if %errorlevel% neq 0 (
    echo ERROR: Compilation failed!
    pause
    exit /b 1
)

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
