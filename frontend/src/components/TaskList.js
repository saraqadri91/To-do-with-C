import React from 'react';
import TaskItem from './TaskItem';

function TaskList({ tasks, onDeleteTask, onUpdateTaskStatus }) {
  if (tasks.length === 0) {
    return null;
  }

  return (
    <div className="space-y-2">
      <h2 className="text-lg font-semibold text-gray-700 mb-4">
        Your Tasks ({tasks.length})
      </h2>
      {tasks.map((task, index) => (
        <TaskItem
          key={task.id}
          task={task}
          onDeleteTask={onDeleteTask}
          onUpdateTaskStatus={onUpdateTaskStatus}
          index={index}
        />
      ))}
    </div>
  );
}

export default TaskList;
