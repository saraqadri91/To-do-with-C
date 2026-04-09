import React, { useState, useEffect } from 'react';

function Mascot({ mood, onAction }) {
  const [currentMood, setCurrentMood] = useState(mood);
  const [isAnimating, setIsAnimating] = useState(false);

  useEffect(() => {
    console.log('Mascot component mounted or mood changed:', { mood, currentMood });
    if (mood !== currentMood) {
      setIsAnimating(true);
      setTimeout(() => {
        setCurrentMood(mood);
        setIsAnimating(false);
      }, 100);
    }
  }, [mood, currentMood]);

  const getMascotEmoji = () => {
    switch (currentMood) {
      case 'happy':
        return '😊';
      case 'excited':
        return '🤗';
      case 'thinking':
        return '🤔';
      case 'sleeping':
        return '😴';
      case 'working':
        return '💼';
      case 'celebrating':
        return '🎉';
      default:
        return '👋';
    }
  };

  const getMascotMessage = () => {
    switch (currentMood) {
      case 'happy':
        return 'Great job!';
      case 'excited':
        return 'Awesome!';
      case 'thinking':
        return 'Hmm...';
      case 'sleeping':
        return 'Zzz...';
      case 'working':
        return 'Working...';
      case 'celebrating':
        return 'Wooo!';
      default:
        return 'Hello!';
    }
  };

  const handleClick = () => {
    setIsAnimating(true);
    setTimeout(() => setIsAnimating(false), 600);
    if (onAction) {
      onAction();
    }
  };

  return (
    <div 
      className={`character ${currentMood} ${isAnimating ? 'bounce' : ''} cursor-pointer select-none p-4 bg-white/20 backdrop-blur-sm rounded-2xl border-2 border-white/30`}
      onClick={handleClick}
      title="Click me!"
      style={{ minWidth: '120px', minHeight: '120px', display: 'flex', alignItems: 'center', justifyContent: 'center' }}
    >
      <div className="relative">
        <div className="text-6xl filter drop-shadow-lg transform transition-all duration-300 hover:scale-110">
          {getMascotEmoji()}
        </div>
        <div className="absolute -top-2 -right-2 bg-gradient-to-r from-pink-400 to-purple-400 text-white text-xs px-2 py-1 rounded-full opacity-0 hover:opacity-100 transition-opacity duration-300">
          {getMascotMessage()}
        </div>
        {/* Fallback text for debugging */}
        <div className="text-xs text-white/80 mt-2 text-center">
          Mood: {currentMood}
        </div>
      </div>
    </div>
  );
}

export default Mascot;
