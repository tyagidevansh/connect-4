const express = require('express');
const cors = require('cors');
const net = require('net');

const app = express();
const serverPort = 3000;
const serverAddress = '127.0.0.1';
const client = new net.Socket();

app.use(cors());
app.use(express.json());

// Connect to the TCP server
client.connect(serverPort, serverAddress, () => {
  console.log('Connected to the server');
});

// Handle connection errors
client.on('error', (error) => {
  console.error('Error:', error.message);
});

// API endpoint to receive data from the React app
app.post('/send-data', (req, res) => {
  const { data } = req.body;

  // Send the received data to the TCP server
  client.write(data);

  // Create a buffer to store the received data
  const receivedData = [];

  // Handle data received from the TCP server
  client.on('data', (data) => {
    console.log('Received from server:', data.toString());
    receivedData.push(data.toString());
  });

  // Send the response after receiving data from the server
  client.once('data', () => {
    res.json({ message: receivedData.join('') });
    receivedData.length = 0; // Clear the buffer
  });
});

app.listen(3001, () => {
  console.log('Express server running on port 3001');
});