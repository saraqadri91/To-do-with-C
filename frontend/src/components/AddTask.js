import React, { useState } from 'react';

function AddTask({ onAddTask, disabled }) {
  const [description, setDescription] = useState('');
  const [isSubmitting, setIsSubmitting] = useState(false);

  const handleSubmit = async (e) => {
    e.preventDefault();
    
    if (!description.trim() || disabled) {
      return;
    }

    setIsSubmitting(true);
    
    try {
      await onAddTask(description.trim());
      setDescription('');
    } finally {
      setIsSubmitting(false);
    }
  };

  return (
    <form onSubmit={handleSubmit} className="flex gap-3">
      <div className="flex-1 relative">
        <input
          type="text"
          value={description}
          onChange={(e) => setDescription(e.target.value)}
          placeholder="What needs to be done? ð"
          className="w-full px-6 py-4 rounded-full bg-white/90 backdrop-filter backdrop-blur-lg text-gray-800 placeholder-gray-500 focus:outline-none focus:ring-4 focus:ring-white/30 focus:bg-white transition-all duration-300 shadow-lg"
          disabled={isSubmitting || disabled}
          maxLength="255"
        />
        <div className="absolute right-4 top-1/2 transform -translate-y-1/2 text-gray-400">
          {description.length}/255
        </div>
      </div>
      <button
        type="submit"
        disabled={isSubmitting || disabled || !description.trim()}
        className="btn bg-gradient-to-r from-yellow-400 to-orange-500 text-white px-8 py-4 rounded-full font-bold text-lg hover:from-yellow-500 hover:to-orange-600 focus:outline-none focus:ring-4 focus:ring-yellow-300 disabled:opacity-50 disabled:cursor-not-allowed transition-all duration-300 shadow-lg flex items-center gap-2"
      >
        {isSubmitting ? (
          <>
            <div className="w-5 h-5 border-3 border-white border-t-transparent rounded-full spinner"></div>
            Adding...
          </>
        ) : (
          <>
            <span className="text-xl">+</span>
            Add Task
          </>
        )}
      </button>
    </form>
  );
}

export default AddTask;
