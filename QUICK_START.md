# Quick Start Guide

## For Windows Users

### 1. Prerequisites Check
Run the test script to check your setup:
```cmd
test-setup.bat
```

### 2. Install C Compiler (if needed)
If the test shows GCC is missing, install one of these:
- **MinGW**: https://www.mingw-w64.org/downloads/
- **WSL**: `wsl --install` in PowerShell
- **Visual Studio Build Tools**: https://visualstudio.microsoft.com/visual-cpp-build-tools/

### 3. Compile C Program
```cmd
cd c-program
compile.bat
```

### 4. Install Dependencies
```cmd
# Backend
cd backend
npm install

# Frontend  
cd ../frontend
npm install
```

### 5. Start Application
**Terminal 1:**
```cmd
cd backend
npm start
```

**Terminal 2:**
```cmd
cd frontend
npm start
```

### 6. Open Browser
Go to: http://localhost:3000

## For Linux/Mac Users

### 1. Install Dependencies
```bash
# Install C compiler (Ubuntu/Debian)
sudo apt-get install build-essential

# Install Node.js (if not installed)
curl -fsSL https://deb.nodesource.com/setup_lts.x | sudo -E bash -
sudo apt-get install -y nodejs
```

### 2. Compile C Program
```bash
cd c-program
gcc todo.c -o todo
```

### 3. Install Node.js Dependencies
```bash
# Backend
cd backend
npm install

# Frontend
cd ../frontend
npm install
```

### 4. Start Application
**Terminal 1:**
```bash
cd backend
npm start
```

**Terminal 2:**
```bash
cd frontend
npm start
```

### 5. Open Browser
Go to: http://localhost:3000

## Testing the Application

1. **Add a task**: Type in the input field and click "Add Task"
2. **Complete a task**: Click the checkbox next to a task
3. **Delete a task**: Click the trash can icon
4. **Check API**: Visit http://localhost:5000/api/tasks

## Troubleshooting

- **Backend not starting**: Check if port 5000 is free
- **Frontend not starting**: Check if port 3000 is free
- **C program errors**: Ensure GCC is installed and compilation succeeded
- **Network errors**: Ensure both servers are running

## Project Structure
```
todo-app/
  backend/          # Node.js API server
  frontend/         # React web app
  c-program/        # C program for task logic
  README.md         # Full documentation
  SETUP.md          # Detailed setup guide
  QUICK_START.md    # This quick start guide
```

## Features Demonstrated
- React with modern UI (Tailwind CSS)
- Node.js/Express REST API
- C program integration via child_process
- File-based data storage
- Error handling and loading states
- Toast notifications
- Responsive design

Happy coding!
