# Setup Guide for To-Do List Application

## Prerequisites

Before you begin, ensure you have the following installed:

1. **Node.js** (v14 or higher)
   - Download from: https://nodejs.org/
   - Verify installation: `node --version`

2. **GCC Compiler** (for C program)
   - Windows: Install MinGW or use WSL
   - macOS: `xcode-select --install`
   - Linux: `sudo apt-get install build-essential`
   - Verify installation: `gcc --version`

3. **Git** (optional, for cloning)
   - Download from: https://git-scm.com/

## Step-by-Step Installation

### 1. Navigate to Project Directory

```bash
cd "c:/Users/Sara Qadri/OneDrive/Desktop/University/c.p lab/project"
```

### 2. Compile the C Program

```bash
cd c-program
gcc todo.c -o todo
# Test the C program
./todo list
./todo add "Test task"
./todo list
cd ..
```

**If compilation fails:**
- Ensure GCC is installed and in your PATH
- On Windows, you might need to use: `gcc todo.c -o todo.exe`

### 3. Install Backend Dependencies

```bash
cd backend
npm install
cd ..
```

### 4. Install Frontend Dependencies

```bash
cd frontend
npm install
cd ..
```

### 5. Start the Application

You need to run both backend and frontend simultaneously.

**Terminal 1 - Start Backend:**
```bash
cd backend
npm start
```
Expected output:
```
Server is running on port 5000
Health check: http://localhost:5000/health
API endpoints: http://localhost:5000/api
```

**Terminal 2 - Start Frontend:**
```bash
cd frontend
npm start
```
Expected output:
```
Starting the development server...
Compiled successfully!
You can now view todo-frontend in the browser.
  Local:            http://localhost:3000
```

### 6. Access the Application

- **Frontend**: http://localhost:3000
- **Backend API**: http://localhost:5000
- **Health Check**: http://localhost:5000/health

## Testing the Application

### Manual Testing

1. **Test Backend API:**
   ```bash
   # Get all tasks
   curl http://localhost:5000/api/tasks
   
   # Add a task
   curl -X POST http://localhost:5000/api/tasks -H "Content-Type: application/json" -d '{"description":"Test task"}'
   
   # Update a task
   curl -X PUT http://localhost:5000/api/tasks/1 -H "Content-Type: application/json" -d '{"completed":true}'
   
   # Delete a task
   curl -X DELETE http://localhost:5000/api/tasks/1
   ```

2. **Test C Program Directly:**
   ```bash
   cd c-program
   ./todo add "Direct C test"
   ./todo list
   ./todo update 1 1
   ./todo delete 1
   ```

3. **Test Frontend:**
   - Open http://localhost:3000 in your browser
   - Try adding, completing, and deleting tasks
   - Check for toast notifications and loading states

## Troubleshooting

### Common Issues

1. **"gcc command not found"**
   - Install GCC compiler
   - Add GCC to your system PATH
   - On Windows, consider using WSL or MinGW

2. **"Cannot connect to backend"**
   - Ensure backend server is running on port 5000
   - Check for firewall issues
   - Verify no other application is using port 5000

3. **"Failed to execute C program"**
   - Ensure the C program is compiled: `todo.exe` (Windows) or `todo` (Linux/Mac)
   - Check file permissions
   - Verify the C program is in the correct directory

4. **"npm install fails"**
   - Clear npm cache: `npm cache clean --force`
   - Delete node_modules and package-lock.json, then try again
   - Check your internet connection

5. **"Port already in use"**
   - Change the port in server.js or kill the process using the port
   - On Windows: `netstat -ano | findstr :5000`
   - On Linux/Mac: `lsof -i :5000`

### File Structure Verification

Ensure your project structure looks like this:
```
todo-app/
  backend/
    node_modules/
    package.json
    server.js
    routes/
      api.js
  frontend/
    node_modules/
    package.json
    public/
      index.html
    src/
      components/
        AddTask.js
        TaskItem.js
        TaskList.js
      App.js
      index.js
      index.css
    tailwind.config.js
    postcss.config.js
  c-program/
    todo.c
    todo.exe  (or todo on Linux/Mac)
    tasks.dat
    Makefile
  README.md
  SETUP.md
```

## Development Tips

### Backend Development
- Use `npm run dev` for auto-restart during development
- Check the console for API request logs
- Test API endpoints with Postman or curl

### Frontend Development
- React hot reload is enabled by default
- Check browser console for errors
- Use React Developer Tools for debugging

### C Program Development
- Use the Makefile: `make` to compile, `make clean` to clean
- Test with `make test` for basic functionality
- Check stderr output for debugging

## Performance Considerations

- The C program uses file-based storage (tasks.dat)
- For production, consider a proper database
- The current implementation supports up to 1000 tasks
- API responses are cached in the frontend state

## Security Notes

- This is a development application, not production-ready
- No authentication is implemented
- No input validation beyond basic checks
- Consider adding CORS restrictions for production

## Next Steps for Enhancement

1. Add user authentication
2. Implement a proper database
3. Add task categories and priorities
4. Implement search and filtering
5. Add due dates and reminders
6. Create a mobile-responsive design
7. Add unit and integration tests
8. Deploy to a cloud platform

## Support

If you encounter any issues:
1. Check this troubleshooting section
2. Verify all prerequisites are installed
3. Ensure all steps were followed in order
4. Check the console/terminal for error messages
5. Test each component individually before integration
