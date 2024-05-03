import React, { useState, useEffect } from 'react';
import './App.css';

function Cell({ value, onCellClick }) {
  return <button className={`cell ${value}`} onClick={onCellClick}>{value}</button>;
}

function App() {
  const boardWidth = 7;
  const boardHeight = 6;
  const [gameState, setGameState] = useState(Array.from({ length: boardHeight }, () => Array(boardWidth).fill('0')));
  const [isRedNext, setIsRedNext] = useState(true);
  const [gameOver, setGameOver] = useState(false);
  const [line, setLine] = useState("");

  useEffect( () => {
    console.log(line);
  }, [line]);

  function handleClick(rowIndex, colIndex) {
    if (gameOver || gameState[rowIndex][colIndex] !== '0') {
      return;
    }

    const copy = [...gameState];
    let lowestEmptyRowIndex = rowIndex;

    while (lowestEmptyRowIndex < boardHeight - 1 && copy[lowestEmptyRowIndex + 1][colIndex] === '0') {
      lowestEmptyRowIndex++;
    }

    copy[lowestEmptyRowIndex][colIndex] = isRedNext ? 'R' : 'B';
    setGameState(copy);
    setLine(line + String(colIndex + 1));

    if (checkForWin(lowestEmptyRowIndex, colIndex)) {
      setGameOver(true);
    }

    setIsRedNext(!isRedNext);
  }

  function checkForWin(rowIndex, colIndex) {
    const token = isRedNext ? 'R' : 'B';
    const directions = [[-1, 0], [1, 0], [0, -1], [0, 1], [-1, -1], [-1, 1], [1, -1], [1, 1]];

    for (const [dx, dy] of directions) {
      let count = 1;
      let row = rowIndex + dx;
      let col = colIndex + dy;

      while (row >= 0 && row < boardHeight && col >= 0 && col < boardWidth && gameState[row][col] === token) {
        count++;
        row += dx;
        col += dy;
      }

      row = rowIndex - dx;
      col = colIndex - dy;

      while (row >= 0 && row < boardHeight && col >= 0 && col < boardWidth && gameState[row][col] === token) {
        count++;
        row -= dx;
        col -= dy;
      }

      if (count >= 4) {
        return true;
      }
    }

    return false;
  }

  return (
    <div className="container">
      <h1>Connect Four</h1>
      <div className="game-board">
        {gameState.map((row, rowIndex) => (
          <div key={rowIndex} className="row">
            {row.map((cell, colIndex) => (
              <Cell key={colIndex} value={cell} onCellClick={() => handleClick(rowIndex, colIndex)} />
            ))}
          </div>
        ))}
        
      </div>
      {gameOver && <div className="game-over">Game Over!</div>}
      
    </div>
  );
}

export default App;