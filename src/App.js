import React, { useState, useEffect } from 'react';
import './App.css';

function Cell({ value, onCellClick, isWinningCell, falling }) {
  return (
    <button
      className={`cell ${value} ${isWinningCell ? 'winning-cell' : ''} ${falling ? 'falling' : ''}`}
      onClick={onCellClick}
    >
      {value}
    </button>
  );
}

function PlayAgain({ onButtonClick }) {
  return <button className='playAgainButton' onClick={onButtonClick}> Reset </button>
}

function App() {
  const boardWidth = 7;
  const boardHeight = 6;
  const [gameState, setGameState] = useState(Array.from({ length: boardHeight }, () => Array(boardWidth).fill('0')));
  const [isRedNext, setIsRedNext] = useState(true);
  const [gameOver, setGameOver] = useState(false);
  const [line, setLine] = useState("");
  const [redStartTime, setRedStartTime] = useState(null);
  const [redDuration, setRedDuration] = useState(0);
  const [yellowDuration, setYellowDuration] = useState(0);
  const [nodesExplored, setNodesExplored] = useState(0);
  const [score, setScore] = useState(0);
  const [winners, setWinners] = useState([]);
  const [scoreMessage, setScoreMessage] = useState(null);
  const [fallingCell, setFallingCell] = useState(null);

  useEffect(() => {
    if (isRedNext) {
      setRedStartTime(Date.now());
    } else {
      sendDataToServer(line);
      setRedDuration(Date.now() - redStartTime);
    }
  }, [line, isRedNext]);

  useEffect(() => {
    console.log(winners);
  }, [winners]);

  function handleClick(rowIndex, colIndex) {
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
    setFallingCell({ rowIndex: lowestEmptyRowIndex, colIndex });
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
    setWinners([]);
    const token = isRedNext ? 'R' : 'Y';
    const directions = [[-1, 0], [1, 0], [0, -1], [0, 1], [-1, -1], [-1, 1], [1, -1], [1, 1]];
    for (const [dx, dy] of directions) {
      let count = 1;
      let row = rowIndex + dx;
      let col = colIndex + dy;
      let arr = [[rowIndex, colIndex]];
      while (row >= 0 && row < boardHeight && col >= 0 && col < boardWidth && gameState[row][col] === token) {
        arr.push([row, col]);
        count++;
        row += dx;
        col += dy;
      }
      row = rowIndex - dx;
      col = colIndex - dy;
      while (row >= 0 && row < boardHeight && col >= 0 && col < boardWidth && gameState[row][col] === token) {
        arr.push([row, col]);
        count++;
        row -= dx;
        col -= dy;
      }
      if (count >= 4) {
        setWinners(arr);
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
    setYellowDuration(arr[2]);
    setScore(arr[13]);
    setNodesExplored(arr[16]);
    setScoreMessageHelper();

    if (!isRedNext) {
      handleClick(0, nextMoveCol - 1);
    }
  }

  function setScoreMessageHelper() {
    if (score === 0) {
      setScoreMessage("The game is evenly balanced right now!");
    } else if (score < 0) {
      setScoreMessage("You will lose in " + Math.abs(score) + " moves, good luck!");
    } else {
      setScoreMessage("You can win in " + score + " moves, good going!");
    }
    setScore(0);
  }

  function isWinningCell(rowIndex, colIndex) {
    return winners.some(([winRow, winCol]) => winRow === rowIndex && winCol === colIndex);
  }

  return (
    <div className="container">
      <h1>Connect Four Solver</h1>

      <div className='mainContent'>

        <div className='redCorner'>
          <h1>🔴 Red Corner 🔴</h1>
          <h2>Time taken for last move:</h2>
          <h3>{redDuration} milliseconds </h3>
          <h2>Game positions explored:</h2>
          <h3>Probably not many </h3>

        </div>

        <div className="game-board">
          {gameState.map((row, rowIndex) => (
            <div key={rowIndex} className="row">
              {row.map((cell, colIndex) => (
                <Cell
                  key={colIndex}
                  value={cell}
                  onCellClick={() => handleClick(rowIndex, colIndex)}
                  isWinningCell={isWinningCell(rowIndex, colIndex)}
                  falling={fallingCell && fallingCell.rowIndex === rowIndex && fallingCell.colIndex === colIndex}
                />
              ))}
            </div>
          ))}
        </div>

        <div className='yellowCorner'>
          <h1>🟡 Yellow Corner 🟡</h1>
          <h2>Time taken for last move: </h2>
          <h3> {yellowDuration} milliseconds </h3>
          <h2>Game positions explored:</h2>
          <h3>{nodesExplored}</h3>
        </div>
      </div>


      <div className='playAgain'>
        <PlayAgain onButtonClick={() => handlePlayAgain()} />
      </div>

      {gameOver && <div className="game-over">Game Over!</div>}
    </div>
  );
}

export default App;
