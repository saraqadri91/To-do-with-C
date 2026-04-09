# Full-Stack To-Do List Application

A modern To-Do List application built with React frontend, Node.js/Express backend, and C program for core logic.

## Architecture

- **Frontend**: React with modern UI (Tailwind CSS)
- **Backend**: Node.js with Express.js
- **Core Logic**: C program with file-based storage (tasks.dat)

## Features

- Add new tasks
- Delete tasks
- Mark tasks as completed
- View all tasks in a styled list
- Modern, responsive UI
- Error handling and loading states

## Project Structure

```
todo-app/
  backend/
    package.json
    server.js
    routes/api.js
  frontend/
    package.json
    public/
    src/
      components/
        TaskList.js
        TaskItem.js
        AddTask.js
      App.js
      index.js
      index.css
  c-program/
    todo.c
    tasks.dat
    Makefile
  README.md
```

## Setup Instructions

### Prerequisites
- Node.js (v14 or higher)
- GCC compiler
- Git

### Installation Steps

1. **Clone/Download the project**
   ```bash
   # Navigate to the project directory
   cd todo-app
   ```

2. **Compile the C program**
   ```bash
   cd c-program
   gcc todo.c -o todo
   cd ..
   ```

3. **Install backend dependencies**
   ```bash
   cd backend
   npm install
   cd ..
   ```

4. **Install frontend dependencies**
   ```bash
   cd frontend
   npm install
   cd ..
   ```

5. **Run the application**
   
   - Start the backend server (Terminal 1):
   ```bash
   cd backend
   npm start
   ```
   
   - Start the frontend (Terminal 2):
   ```bash
   cd frontend
   npm start
   ```

6. **Access the application**
   - Frontend: http://localhost:3000
   - Backend API: http://localhost:5000

## How It Works

1. **Frontend** sends HTTP requests to the backend
2. **Backend** receives requests and calls the C program
3. **C program** performs file operations on tasks.dat
4. **Backend** returns JSON response to frontend
5. **Frontend** updates the UI based on the response

## API Endpoints

- `GET /api/tasks` - Get all tasks
- `POST /api/tasks` - Add a new task
- `DELETE /api/tasks/:id` - Delete a task
- `PUT /api/tasks/:id` - Update task completion status

## File Storage

Tasks are stored in `c-program/tasks.dat` using a simple binary format managed by the C program.

## Technologies Used

- **Frontend**: React, Tailwind CSS
- **Backend**: Node.js, Express.js
- **Core Logic**: C programming language
- **Communication**: REST APIs, child_process
