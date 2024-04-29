import './App.css';
import {useState} from 'react';

function Cell({value, onCellClick}) {
  //var value = 0;
  return <button className='gridCell' onClick = {onCellClick}> {value} </button>
}

function App() {
  const boardWidth = 7;
  const boardHeight = 6;
  const [gameState, setGameState] = useState(Array.from({ length: boardHeight }, () => Array(boardWidth).fill('')));
  console.log(gameState);
  
  function handleClick(rowIndex, colIndex) {
    const copy = [...gameState];
    copy[rowIndex][colIndex] = 'X';
    setGameState(copy);
    console.log(gameState);
  }

  return (
    <>
      {gameState.map((row, rowIndex) => (
        <div key = {rowIndex} className='row'>
          {row.map((cell, colIndex) => (
            <Cell key = {colIndex} value = {cell} onClick = {() => handleClick(rowIndex, colIndex)}/>
          ))}
        </div>
      ))}
    </>
  );
}

export default App;
