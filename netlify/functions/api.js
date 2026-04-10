// Netlify serverless function for tasks API
const express = require('express');
const serverless = require('serverless-http');
const cors = require('cors');
const helmet = require('helmet');

// Mock in-memory storage for Netlify deployment
let tasks = [];
let nextId = 1;

const app = express();

// Middleware
app.use(helmet());
app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// GET /api/tasks - Get all tasks
app.get('/tasks', (req, res) => {
  res.json({
    success: true,
    data: tasks
  });
});

// POST /api/tasks - Create new task
app.post('/tasks', (req, res) => {
  const { description } = req.body;
  
  if (!description || description.trim() === '') {
    return res.status(400).json({
      success: false,
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
    data: newTask
  });
});

// PUT /api/tasks/:id - Update task
app.put('/tasks/:id', (req, res) => {
  const { id } = req.params;
  const { completed } = req.body;
  
  const taskIndex = tasks.findIndex(task => task.id === parseInt(id));
  
  if (taskIndex === -1) {
    return res.status(404).json({
      success: false,
      message: 'Task not found'
    });
  }

  tasks[taskIndex].completed = Boolean(completed);

  res.json({
    success: true,
    data: tasks[taskIndex]
  });
});

// DELETE /api/tasks/:id - Delete task
app.delete('/tasks/:id', (req, res) => {
  const { id } = req.params;
  
  const taskIndex = tasks.findIndex(task => task.id === parseInt(id));
  
  if (taskIndex === -1) {
    return res.status(404).json({
      success: false,
      message: 'Task not found'
    });
  }

  const deletedTask = tasks.splice(taskIndex, 1)[0];

  res.json({
    success: true,
    data: deletedTask
  });
});

// Health check endpoint
app.get('/health', (req, res) => {
  res.json({
    status: 'healthy',
    timestamp: new Date().toISOString(),
    tasks: tasks.length
  });
});

// Error handling middleware
app.use((err, req, res, next) => {
  console.error(err.stack);
  res.status(500).json({
    success: false,
    message: 'Internal server error'
  });
});

// 404 handler
app.use((req, res) => {
  res.status(404).json({
    success: false,
    message: 'Endpoint not found'
  });
});

module.exports.handler = serverless(app);
