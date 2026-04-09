const express = require('express');
const { exec } = require('child_process');
const path = require('path');
const fs = require('fs');

const router = express.Router();

// Path to the C program executable
const C_PROGRAM_PATH = path.join(__dirname, '../c-program/todo');
const C_PROGRAM_DIR = path.join(__dirname, '../c-program');

// Helper function to execute C program
function executeCProgram(command, args = []) {
    return new Promise((resolve, reject) => {
        const fullCommand = `${C_PROGRAM_PATH} ${command} ${args.join(' ')}`;
        
        exec(fullCommand, {
            cwd: C_PROGRAM_DIR,
            timeout: 5000 // 5 second timeout
        }, (error, stdout, stderr) => {
            if (error) {
                console.error('C program execution error:', error);
                return reject(new Error('Failed to execute C program'));
            }
            
            if (stderr) {
                console.error('C program stderr:', stderr);
                // Don't reject on stderr, as the C program prints errors to stderr
                // but still returns JSON output
            }
            
            try {
                const result = JSON.parse(stdout.trim());
                resolve(result);
            } catch (parseError) {
                console.error('JSON parse error:', parseError);
                console.error('Raw stdout:', stdout);
                reject(new Error('Failed to parse C program output'));
            }
        });
    });
}

// GET /api/tasks - Get all tasks
router.get('/tasks', async (req, res) => {
    try {
        const tasks = await executeCProgram('list');
        res.json({
            success: true,
            data: tasks,
            count: tasks.length
        });
    } catch (error) {
        console.error('Error fetching tasks:', error);
        res.status(500).json({
            success: false,
            error: 'Failed to fetch tasks',
            message: error.message
        });
    }
});

// POST /api/tasks - Add a new task
router.post('/tasks', async (req, res) => {
    try {
        const { description } = req.body;
        
        // Validate input
        if (!description || description.trim() === '') {
            return res.status(400).json({
                success: false,
                error: 'Bad Request',
                message: 'Task description is required'
            });
        }
        
        if (description.length > 255) {
            return res.status(400).json({
                success: false,
                error: 'Bad Request',
                message: 'Task description must be 255 characters or less'
            });
        }
        
        // Escape quotes in description for command line
        const escapedDescription = description.replace(/"/g, '\\"');
        
        const tasks = await executeCProgram('add', [escapedDescription]);
        
        // Find the newly added task (it will be the last one)
        const newTask = tasks[tasks.length - 1];
        
        res.status(201).json({
            success: true,
            data: newTask,
            message: 'Task added successfully'
        });
    } catch (error) {
        console.error('Error adding task:', error);
        res.status(500).json({
            success: false,
            error: 'Failed to add task',
            message: error.message
        });
    }
});

// DELETE /api/tasks/:id - Delete a task
router.delete('/tasks/:id', async (req, res) => {
    try {
        const taskId = req.params.id;
        
        // Validate ID
        if (!taskId || isNaN(taskId)) {
            return res.status(400).json({
                success: false,
                error: 'Bad Request',
                message: 'Valid task ID is required'
            });
        }
        
        const tasks = await executeCProgram('delete', [taskId]);
        
        res.json({
            success: true,
            data: tasks,
            message: 'Task deleted successfully'
        });
    } catch (error) {
        console.error('Error deleting task:', error);
        res.status(500).json({
            success: false,
            error: 'Failed to delete task',
            message: error.message
        });
    }
});

// PUT /api/tasks/:id - Update task completion status
router.put('/tasks/:id', async (req, res) => {
    try {
        const taskId = req.params.id;
        const { completed } = req.body;
        
        // Validate inputs
        if (!taskId || isNaN(taskId)) {
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
        
        // Convert boolean to 0/1 for C program
        const completedValue = completed ? 1 : 0;
        
        const tasks = await executeCProgram('update', [taskId, completedValue]);
        
        // Find the updated task
        const updatedTask = tasks.find(task => task.id === parseInt(taskId));
        
        res.json({
            success: true,
            data: updatedTask,
            message: 'Task updated successfully'
        });
    } catch (error) {
        console.error('Error updating task:', error);
        res.status(500).json({
            success: false,
            error: 'Failed to update task',
            message: error.message
        });
    }
});

// Error handling middleware for this router
router.use((error, req, res, next) => {
    console.error('API route error:', error);
    res.status(500).json({
        success: false,
        error: 'Internal Server Error',
        message: 'An unexpected error occurred'
    });
});

module.exports = router;
