import './App.css';
import {useState} from 'react';

function Cell({value, onCellClick}) {
  //var value = 0;
  return <button className='gridCell' onClick = {onCellClick}> {value} </button>
}

function App() {
  const boardWidth = 7;
  const boardHeight = 6;
  const [gameState, setGameState] = useState(Array.from({ length: boardHeight }, () => Array(boardWidth).fill('0')));
  const [isRedNext, setIsRedNext] = useState(true);
  console.log(gameState);
  
  function handleClick(rowIndex, colIndex) {
    const copy = [...gameState];

    let lowestEmptyRowIndex = rowIndex;
    while (lowestEmptyRowIndex < boardHeight - 1 && copy[lowestEmptyRowIndex + 1][colIndex] === '0') {
      lowestEmptyRowIndex++;
    }
  
    copy[lowestEmptyRowIndex][colIndex] = isRedNext ? 'R' : 'B';
    setGameState(copy);
    setIsRedNext(!isRedNext);
  }

  return (
    <>
      <div className='row'>
        <Cell value = {gameState[0][0]} onCellClick = {() => {handleClick(0, 0)}}/>
        <Cell value = {gameState[0][1]} onCellClick = {() => {handleClick(0, 1)}}/>
        <Cell value = {gameState[0][2]} onCellClick = {() => {handleClick(0, 2)}}/>
        <Cell value = {gameState[0][3]} onCellClick = {() => {handleClick(0, 3)}}/>
        <Cell value = {gameState[0][4]} onCellClick = {() => {handleClick(0, 4)}}/>
        <Cell value = {gameState[0][5]} onCellClick = {() => {handleClick(0, 5)}}/>
        <Cell value = {gameState[0][6]} onCellClick = {() => {handleClick(0, 6)}}/>
      </div>

      <div className='row'>
        <Cell value = {gameState[1][0]} onCellClick = {() => {handleClick(1, 0)}}/>
        <Cell value = {gameState[1][1]} onCellClick = {() => {handleClick(1, 1)}}/>
        <Cell value = {gameState[1][2]} onCellClick = {() => {handleClick(1, 2)}}/>
        <Cell value = {gameState[1][3]} onCellClick = {() => {handleClick(1, 3)}}/>
        <Cell value = {gameState[1][4]} onCellClick = {() => {handleClick(1, 4)}}/>
        <Cell value = {gameState[1][5]} onCellClick = {() => {handleClick(1, 5)}}/>
        <Cell value = {gameState[1][6]} onCellClick = {() => {handleClick(1, 6)}}/>
      </div>

      <div className='row'>
        <Cell value = {gameState[2][0]} onCellClick = {() => {handleClick(2, 0)}}/>
        <Cell value = {gameState[2][1]} onCellClick = {() => {handleClick(2, 1)}}/>
        <Cell value = {gameState[2][2]} onCellClick = {() => {handleClick(2, 2)}}/>
        <Cell value = {gameState[2][3]} onCellClick = {() => {handleClick(2, 3)}}/>
        <Cell value = {gameState[2][4]} onCellClick = {() => {handleClick(2, 4)}}/>
        <Cell value = {gameState[2][5]} onCellClick = {() => {handleClick(2, 5)}}/>
        <Cell value = {gameState[2][6]} onCellClick = {() => {handleClick(2, 6)}}/>
      </div>

      <div className='row'>
        <Cell value = {gameState[3][0]} onCellClick = {() => {handleClick(3, 0)}}/>
        <Cell value = {gameState[3][1]} onCellClick = {() => {handleClick(3, 1)}}/>
        <Cell value = {gameState[3][2]} onCellClick = {() => {handleClick(3, 2)}}/>
        <Cell value = {gameState[3][3]} onCellClick = {() => {handleClick(3, 3)}}/>
        <Cell value = {gameState[3][4]} onCellClick = {() => {handleClick(3, 4)}}/>
        <Cell value = {gameState[3][5]} onCellClick = {() => {handleClick(3, 5)}}/>
        <Cell value = {gameState[3][6]} onCellClick = {() => {handleClick(3, 6)}}/>
      </div>

      <div className='row'>
        <Cell value = {gameState[4][0]} onCellClick = {() => {handleClick(4, 0)}}/>
        <Cell value = {gameState[4][1]} onCellClick = {() => {handleClick(4, 1)}}/>
        <Cell value = {gameState[4][2]} onCellClick = {() => {handleClick(4, 2)}}/>
        <Cell value = {gameState[4][3]} onCellClick = {() => {handleClick(4, 3)}}/>
        <Cell value = {gameState[4][4]} onCellClick = {() => {handleClick(4, 4)}}/>
        <Cell value = {gameState[4][5]} onCellClick = {() => {handleClick(4, 5)}}/>
        <Cell value = {gameState[4][6]} onCellClick = {() => {handleClick(4, 6)}}/>
      </div>

      <div className='row'>
        <Cell value = {gameState[5][0]} onCellClick = {() => {handleClick(5, 0)}}/>
        <Cell value = {gameState[5][1]} onCellClick = {() => {handleClick(5, 1)}}/>
        <Cell value = {gameState[5][2]} onCellClick = {() => {handleClick(5, 2)}}/>
        <Cell value = {gameState[5][3]} onCellClick = {() => {handleClick(5, 3)}}/>
        <Cell value = {gameState[5][4]} onCellClick = {() => {handleClick(5, 4)}}/>
        <Cell value = {gameState[5][5]} onCellClick = {() => {handleClick(5, 5)}}/>
        <Cell value = {gameState[5][6]} onCellClick = {() => {handleClick(5, 6)}}/>
      </div>
    </>
  );
}

export default App;
