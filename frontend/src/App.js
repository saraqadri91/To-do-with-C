import React, { useState, useEffect, useCallback } from 'react';
import TaskList from './components/TaskList';
import AddTask from './components/AddTask';
import Mascot from './components/Mascot';
import './index.css';

function App() {
  const [tasks, setTasks] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);
  const [toast, setToast] = useState(null);
  const [mascotMood, setMascotMood] = useState('happy');
  const [isAddingTask, setIsAddingTask] = useState(false);

  // Show toast notification
  const showToast = useCallback((message, type = 'info') => {
    setToast({ message, type });
    setTimeout(() => setToast(null), 3000);
  }, []);

  // Update mascot mood based on action
  const updateMascotMood = useCallback((mood) => {
    setMascotMood(mood);
    setTimeout(() => setMascotMood('happy'), 2000);
  }, []);

  // Fetch all tasks from API
  const fetchTasks = useCallback(async () => {
    try {
      setLoading(true);
      updateMascotMood('working');
      const response = await fetch('/api/tasks');
      const data = await response.json();
      
      if (data.success) {
        setTasks(data.data);
        setError(null);
        updateMascotMood(data.data.length > 0 ? 'happy' : 'thinking');
      } else {
        setError(data.message || 'Failed to fetch tasks');
        updateMascotMood('sleeping');
      }
    } catch (err) {
      setError('Network error. Please check if the backend server is running.');
      console.error('Error fetching tasks:', err);
      updateMascotMood('sleeping');
    } finally {
      setLoading(false);
    }
  }, []);

  // Add a new task
  const addTask = async (description) => {
    setIsAddingTask(true);
    updateMascotMood('working');
    
    try {
      const response = await fetch('/api/tasks', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ description }),
      });
      
      const data = await response.json();
      
      if (data.success) {
        setTasks([...tasks, data.data]);
        showToast('Task added successfully!', 'success');
        updateMascotMood('excited');
      } else {
        showToast(data.message || 'Failed to add task', 'error');
        updateMascotMood('thinking');
      }
    } catch (err) {
      showToast('Network error. Failed to add task.', 'error');
      console.error('Error adding task:', err);
      updateMascotMood('sleeping');
    } finally {
      setIsAddingTask(false);
    }
  };

  // Delete a task
  const deleteTask = async (id) => {
    updateMascotMood('working');
    
    try {
      const response = await fetch(`/api/tasks/${id}`, {
        method: 'DELETE',
      });
      
      const data = await response.json();
      
      if (data.success) {
        setTasks(data.data);
        showToast('Task deleted successfully!', 'success');
        updateMascotMood('happy');
      } else {
        showToast(data.message || 'Failed to delete task', 'error');
        updateMascotMood('thinking');
      }
    } catch (err) {
      showToast('Network error. Failed to delete task.', 'error');
      console.error('Error deleting task:', err);
      updateMascotMood('sleeping');
    }
  };

  // Update task completion status
  const updateTaskStatus = async (id, completed) => {
    updateMascotMood('working');
    
    try {
      const response = await fetch(`/api/tasks/${id}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ completed }),
      });
      
      const data = await response.json();
      
      if (data.success) {
        setTasks(tasks.map(task => 
          task.id === id ? data.data : task
        ));
        showToast(`Task marked as ${completed ? 'completed' : 'incomplete'}!`, 'success');
        updateMascotMood(completed ? 'celebrating' : 'happy');
      } else {
        showToast(data.message || 'Failed to update task', 'error');
        updateMascotMood('thinking');
      }
    } catch (err) {
      showToast('Network error. Failed to update task.', 'error');
      console.error('Error updating task:', err);
      updateMascotMood('sleeping');
    }
  };

  // Load tasks on component mount
  useEffect(() => {
    fetchTasks();
  }, [fetchTasks]);

  const handleMascotClick = () => {
    console.log('Mascot clicked! Current mood:', mascotMood);
    updateMascotMood('excited');
    showToast('Thanks for clicking me! 😊', 'info');
  };

  return (
    <div className="min-h-screen py-8 px-4 relative overflow-hidden">
      {/* Animated Background Elements */}
      <div className="absolute inset-0 overflow-hidden">
        <div className="absolute top-20 left-20 w-32 h-32 bg-gradient-to-r from-pink-400 to-purple-400 rounded-full opacity-20 float"></div>
        <div className="absolute top-40 right-20 w-24 h-24 bg-gradient-to-r from-blue-400 to-cyan-400 rounded-full opacity-20 float" style={{ animationDelay: '1s' }}></div>
        <div className="absolute bottom-20 left-40 w-40 h-40 bg-gradient-to-r from-yellow-400 to-orange-400 rounded-full opacity-20 float" style={{ animationDelay: '2s' }}></div>
      </div>

      <div className="max-w-4xl mx-auto relative z-10">
        {/* Header with Mascot */}
        <div className="text-center mb-8 fade-in">
          <div className="flex items-center justify-center gap-4 mb-4">
            <Mascot mood={mascotMood} onAction={handleMascotClick} />
            <div>
              <h1 className="text-5xl font-bold gradient-text mb-2">
                To-Do List App
              </h1>
              <p className="text-white/90 text-lg font-medium">
                Organize your tasks with style and fun! ð
              </p>
            </div>
            <div className="w-16"></div>
          </div>
        </div>

        {/* Main Card with Glass Morphism */}
        <div className="glass shadow-2xl overflow-hidden fade-in p-8">
          {/* Add Task Section */}
          <div className="mb-8">
            <div className="bg-gradient-to-r from-pink-500 via-purple-500 to-indigo-500 p-6 rounded-2xl shadow-lg">
              <h2 className="text-white text-xl font-bold mb-4 text-center">Add New Task</h2>
              <AddTask onAddTask={addTask} disabled={isAddingTask} />
            </div>
          </div>

          {/* Task List Section */}
          <div className="min-h-[300px]">
            {loading ? (
              <div className="text-center py-12">
                <div className="inline-block w-12 h-12 border-4 border-white border-t-transparent rounded-full spinner mb-4"></div>
                <p className="text-white text-lg font-medium">Loading tasks...</p>
                <Mascot mood="working" />
              </div>
            ) : error ? (
              <div className="text-center py-12">
                <div className="text-red-400 mb-6">
                  <svg className="w-20 h-20 mx-auto" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                    <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M12 8v4m0 4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z" />
                  </svg>
                </div>
                <p className="text-white mb-6 text-lg">{error}</p>
                <button
                  onClick={fetchTasks}
                  className="btn bg-gradient-to-r from-blue-500 to-purple-500 text-white px-8 py-3 rounded-full font-semibold text-lg"
                >
                  Retry
                </button>
                <Mascot mood="sleeping" />
              </div>
            ) : tasks.length === 0 ? (
              <div className="text-center py-12">
                <div className="text-white/60 mb-6">
                  <svg className="w-20 h-20 mx-auto" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                    <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M9 5H7a2 2 0 00-2 2v12a2 2 0 002 2h10a2 2 0 002-2V7a2 2 0 00-2-2h-2M9 5a2 2 0 002 2h2a2 2 0 002-2M9 5a2 2 0 012-2h2a2 2 0 012 2" />
                  </svg>
                </div>
                <p className="text-white/80 text-lg mb-4">No tasks yet!</p>
                <p className="text-white/60">Add your first task above to get started ð</p>
                <Mascot mood="thinking" />
              </div>
            ) : (
              <div>
                <div className="flex items-center justify-between mb-6">
                  <h2 className="text-2xl font-bold gradient-text">Your Tasks ({tasks.length})</h2>
                  <div className="flex items-center gap-2">
                    <span className="text-white/80">Completed:</span>
                    <span className="bg-gradient-to-r from-green-400 to-blue-500 text-white px-3 py-1 rounded-full font-semibold">
                      {tasks.filter(t => t.completed).length}/{tasks.length}
                    </span>
                  </div>
                </div>
                <TaskList
                  tasks={tasks}
                  onDeleteTask={deleteTask}
                  onUpdateTaskStatus={updateTaskStatus}
                />
              </div>
            )}
          </div>
        </div>

        {/* Footer */}

      </div>

      {/* Toast Notification */}
      {toast && (
        <div className={`toast ${toast.type}`}>
          <div className="flex items-center gap-2">
            <span className="text-xl">{toast.type === 'success' ? 'ð' : toast.type === 'error' ? 'ð' : 'ð'}</span>
            <span>{toast.message}</span>
          </div>
        </div>
      )}
    </div>
  );
}

export default App;
