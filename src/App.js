import React, { useState, useEffect } from 'react';
import './App.css';

function Cell({ value, onCellClick, isAnimating, lastCell, rowIndex, colIndex }) {
  const isLastCell = lastCell && lastCell.rowIndex == rowIndex && lastCell.colIndex == colIndex;
  console.log(isLastCell, rowIndex, colIndex);
  return (
    <button
    className={`cell ${value} ${isAnimating && isLastCell ? 'fall-animation' : ''}`}
      onClick={onCellClick}
      disabled={isAnimating}
    >
      {value}
    </button>
  );
}

function PlayAgain({onButtonClick}) {
  return <button className='playAgainButton' onClick={onButtonClick}> Reset </button>
}

function App() {
  const boardWidth = 7;
  const boardHeight = 6;
  const [gameState, setGameState] = useState(Array.from({ length: boardHeight }, () => Array(boardWidth).fill('0')));
  const [isRedNext, setIsRedNext] = useState(true);
  const [gameOver, setGameOver] = useState(false);
  const [line, setLine] = useState("");
  const [isAnimating, setIsAnimating] = useState(false);
  const [lastCell, setLastCell] = useState(null);

  useEffect(() => {
    console.log(line);
    if (!isRedNext) {
      sendDataToServer(line);
    }
  }, [line, isRedNext]);

  function handleClick(rowIndex, colIndex) {
    setLastCell({rowIndex, colIndex});
    setIsAnimating(true); // Set animation flag to true
    setTimeout(() => {
      setIsAnimating(false); // Reset animation flag after animation duration
    }, 500);

    const copy = [...gameState];

    let lowestEmptyRowIndex = rowIndex;
    while (lowestEmptyRowIndex < boardHeight - 1 && copy[lowestEmptyRowIndex + 1][colIndex] === '0') {
      lowestEmptyRowIndex++;
    }
    if (copy[lowestEmptyRowIndex][colIndex] === 'R' || copy[lowestEmptyRowIndex][colIndex] === 'Y') {
      return;
    }
    copy[lowestEmptyRowIndex][colIndex] = isRedNext ? 'R' : 'Y';
    setGameState(copy);
    setLine(line + String(colIndex + 1));
    if (checkForWin(lowestEmptyRowIndex, colIndex)) {
      setGameOver(true);
    }
    setIsRedNext(!isRedNext);
  }

  function handlePlayAgain() {
    const copy = [...gameState];  
    for (let i = 0; i < boardHeight; i++) {
      for (let j = 0; j < boardWidth; j++) {
        copy[i][j] = '0';
      }
    }
    setGameState(copy);
    setLine("");
    setIsRedNext(true);
    setGameOver(false);
  }

  function checkForWin(rowIndex, colIndex) {
    const token = isRedNext ? 'R' : 'Y';
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

  function sendDataToServer(data) {
    fetch('http://localhost:3001/send-data', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ data }),
    })
      .then(response => response.json())
      .then(data => getScore(data.message))
      .catch(error => console.error('Error:', error));
  }

  function getScore(data) {
    const arr = data.split(" ");
    const nextMoveCol = Number(arr[6]);
    console.log(nextMoveCol);

    if (!isRedNext) {
      handleClick(0, nextMoveCol-1);
    }
  }

  return (
    <div className="container">
      <h1>Connect Four Solver</h1>
      <div className="game-board">
        {gameState.map((row, rowIndex) => (
          <div key={rowIndex} className="row">
            {row.map((cell, colIndex) => (
              <Cell key={colIndex} value={cell} onCellClick={() => handleClick(rowIndex, colIndex)} isAnimating = {isAnimating} lastCell = {lastCell} rowIndex = {rowIndex} colIndex = {colIndex}/>
            ))}
          </div>
        ))}
      </div>
      {gameOver && <div className="game-over">Game Over!</div>}
      <div className='playAgain'>
        <PlayAgain onButtonClick={() => handlePlayAgain()}/>
      </div>
    </div>
  );
}

export default App;