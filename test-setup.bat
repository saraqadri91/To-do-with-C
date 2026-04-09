@echo off
echo ========================================
echo Testing To-Do List Application Setup
echo ========================================
echo.

REM Check Node.js
echo [1/4] Checking Node.js...
node --version >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: Node.js not found!
    echo Please install Node.js from https://nodejs.org/
    pause
    exit /b 1
) else (
    echo SUCCESS: Node.js is installed
    node --version
)
echo.

REM Check npm
echo [2/4] Checking npm...
npm --version >nul 2>nul
if %errorlevel% neq 0 (
    echo ERROR: npm not found!
    pause
    exit /b 1
) else (
    echo SUCCESS: npm is installed
    npm --version
)
echo.

REM Check C compiler
echo [3/4] Checking C compiler...
where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo WARNING: GCC compiler not found!
    echo You will need to install GCC to compile the C program.
    echo.
    echo Options:
    echo - Install MinGW: https://www.mingw-w64.org/
    echo - Use WSL (Windows Subsystem for Linux)
    echo - Use Visual Studio Build Tools
    echo.
) else (
    echo SUCCESS: GCC compiler is available
    gcc --version | head -1
)
echo.

REM Check project structure
echo [4/4] Checking project structure...
if exist "backend\package.json" (
    echo SUCCESS: Backend package.json found
) else (
    echo ERROR: Backend package.json not found!
)

if exist "frontend\package.json" (
    echo SUCCESS: Frontend package.json found
) else (
    echo ERROR: Frontend package.json not found!
)

if exist "c-program\todo.c" (
    echo SUCCESS: C program source found
) else (
    echo ERROR: C program source not found!
)
echo.

echo ========================================
echo Setup Check Complete!
echo ========================================
echo.

REM Next steps
echo Next Steps:
echo 1. If GCC is not installed, install it first
echo 2. Compile the C program: cd c-program && compile.bat
echo 3. Install dependencies:
echo    - Backend: cd backend && npm install
echo    - Frontend: cd frontend && npm install
echo 4. Start the servers:
echo    - Backend: cd backend && npm start
echo    - Frontend: cd frontend && npm start
echo 5. Open http://localhost:3000 in your browser
echo.

pause
