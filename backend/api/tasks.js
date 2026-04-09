// Vercel serverless function for tasks API
const express = require('express');
const cors = require('cors');
const helmet = require('helmet');

// Mock in-memory storage for Vercel deployment
let tasks = [];
let nextId = 1;

const app = express();

// Middleware
app.use(helmet());
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// GET /api/tasks - Get all tasks
app.get('/', (req, res) => {
  res.json({
    success: true,
    data: tasks,
    count: tasks.length
  });
});

// POST /api/tasks - Add a new task
app.post('/', (req, res) => {
  const { description } = req.body;
  
  if (!description || description.trim() === '') {
    return res.status(400).json({
      success: false,
      error: 'Bad Request',
      message: 'Task description is required'
    });
  }
  
  const newTask = {
    id: nextId++,
    description: description.trim(),
    completed: false,
    created_at: Math.floor(Date.now() / 1000)
  };
  
  tasks.push(newTask);
  
  res.status(201).json({
    success: true,
    data: newTask,
    message: 'Task added successfully'
  });
});

// DELETE /api/tasks/:id - Delete a task
app.delete('/:id', (req, res) => {
  const taskId = parseInt(req.params.id);
  
  if (isNaN(taskId)) {
    return res.status(400).json({
      success: false,
      error: 'Bad Request',
      message: 'Valid task ID is required'
    });
  }
  
  const initialLength = tasks.length;
  tasks = tasks.filter(task => task.id !== taskId);
  
  if (tasks.length === initialLength) {
    return res.status(404).json({
      success: false,
      error: 'Not Found',
      message: 'Task not found'
    });
  }
  
  res.json({
    success: true,
    data: tasks,
    message: 'Task deleted successfully'
  });
});

// PUT /api/tasks/:id - Update task completion status
app.put('/:id', (req, res) => {
  const taskId = parseInt(req.params.id);
  const { completed } = req.body;
  
  if (isNaN(taskId)) {
    return res.status(400).json({
      success: false,
      error: 'Bad Request',
      message: 'Valid task ID is required'
    });
  }
  
  if (typeof completed !== 'boolean') {
    return res.status(400).json({
      success: false,
      error: 'Bad Request',
      message: 'Completed status must be true or false'
    });
  }
  
  const task = tasks.find(task => task.id === taskId);
  
  if (!task) {
    return res.status(404).json({
      success: false,
      error: 'Not Found',
      message: 'Task not found'
    });
  }
  
  task.completed = completed;
  
  res.json({
    success: true,
    data: task,
    message: 'Task updated successfully'
  });
});

// Export for Vercel
module.exports = app;
