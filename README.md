# Connect 4 Solver

## Description
This is a basic implementation of a Connect 4 solver using the negamax variant of the minimax algrithm with alpha-beta pruning. The frontend of the application is developed using React. The backend, which powers the solver, is written in C++ and utilizes Winsock for communication. Node's Express and NET modules are used to facilitate communication between the frontend and backend.


## Setup Instructions

### Prerequisites
Ensure you have the following installed on your system:
- Git
- Node.js and npm
- A C++ compiler
- Makefile

This project utilises winsock2, so changes would be required to make it run on any other OS.

### Steps to Start the Application

1. **Clone the Repository**
   ```bash
   git clone https://github.com/tyagidevansh/connect-4
   cd connect-4
2. **Start the React App in Development Mode**
    ```bash
    npm install
    npm start
3. Start the C++ server
    ```bash
    cd src/solver
    make
    connect4_server.exe
4. Run the node client
    ```bash
    node client.js

Enjoy!