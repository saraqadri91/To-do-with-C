import React, { useState } from 'react';

function TaskItem({ task, onDeleteTask, onUpdateTaskStatus, index }) {
  const [isDeleting, setIsDeleting] = useState(false);
  const [isUpdating, setIsUpdating] = useState(false);
  const [isHovered, setIsHovered] = useState(false);

  const handleDelete = async () => {
    if (isDeleting || isUpdating) return;
    
    setIsDeleting(true);
    try {
      await onDeleteTask(task.id);
    } finally {
      setIsDeleting(false);
    }
  };

  const handleToggleComplete = async () => {
    if (isDeleting || isUpdating) return;
    
    setIsUpdating(true);
    try {
      await onUpdateTaskStatus(task.id, !task.completed);
    } finally {
      setIsUpdating(false);
    }
  };

  const formatDate = (timestamp) => {
    const date = new Date(timestamp * 1000);
    return date.toLocaleDateString() + ' ' + date.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
  };

  const getTaskEmoji = () => {
    if (task.completed) return 'ð';
    return 'ð';
  };

  return (
    <div 
      className={`task-item rounded-2xl p-5 border-2 slide-in ${
        task.completed 
          ? 'bg-gradient-to-r from-green-50 to-blue-50 border-green-200 opacity-90' 
          : 'bg-white border-gray-200'
      } ${isHovered ? 'shadow-xl' : 'shadow-md'} transition-all duration-300`}
      style={{ animationDelay: `${index * 0.05}s` }}
      onMouseEnter={() => setIsHovered(true)}
      onMouseLeave={() => setIsHovered(false)}
    >
      <div className="flex items-center gap-4">
        {/* Task Emoji */}
        <div className="text-3xl transform transition-transform duration-300 hover:scale-110">
          {getTaskEmoji()}
        </div>

        {/* Checkbox */}
        <button
          onClick={handleToggleComplete}
          disabled={isDeleting || isUpdating}
          className={`checkbox-custom flex-shrink-0 ${
            task.completed 
              ? 'bg-gradient-to-r from-green-400 to-blue-500 border-green-400' 
              : 'bg-white border-pink-400'
          } ${isUpdating ? 'opacity-50 cursor-not-allowed' : 'hover:scale-110'}`}
          aria-label={task.completed ? 'Mark as incomplete' : 'Mark as complete'}
        >
          {task.completed && (
            <span className="text-white font-bold">â</span>
          )}
        </button>

        {/* Task Content */}
        <div className="flex-1 min-w-0">
          <p className={`text-lg break-words font-medium transition-all duration-300 ${
            task.completed 
              ? 'line-through text-gray-500' 
              : 'text-gray-800'
          }`}>
            {task.description}
          </p>
          <div className="flex items-center gap-4 mt-2">
            <p className="text-xs text-gray-500 flex items-center gap-1">
              <span>ð</span>
              {formatDate(task.created_at)}
            </p>
            {task.completed && (
              <span className="text-xs bg-gradient-to-r from-green-400 to-blue-500 text-white px-2 py-1 rounded-full font-semibold">
                Completed! ð
              </span>
            )}
          </div>
        </div>

        {/* Delete Button */}
        <button
          onClick={handleDelete}
          disabled={isDeleting || isUpdating}
          className={`btn flex-shrink-0 p-3 rounded-2xl transition-all duration-300 ${
            isDeleting 
              ? 'bg-gray-300 cursor-not-allowed' 
              : 'bg-gradient-to-r from-red-400 to-pink-500 hover:from-red-500 hover:to-pink-600 text-white hover:scale-110'
          }`}
          aria-label="Delete task"
        >
          {isDeleting ? (
            <div className="w-5 h-5 border-3 border-white border-t-transparent rounded-full spinner"></div>
          ) : (
            <svg className="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M19 7l-.867 12.142A2 2 0 0116.138 21H7.862a2 2 0 01-1.995-1.858L5 7m5 4v6m4-6v6m1-10V4a1 1 0 00-1-1h-4a1 1 0 00-1 1v3M4 7h16" />
            </svg>
          )}
        </button>
      </div>
    </div>
  );
}

export default TaskItem;
