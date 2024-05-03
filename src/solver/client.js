const net = require('net');

// Define the server details
const serverPort = 8080;
const serverAddress = '127.0.0.1'; 

// Create a new TCP client
const client = new net.Socket();

// Connect to the server
client.connect(serverPort, serverAddress, () => {
    console.log('Connected to the server');

    // Send some data to the server
    client.write('Hello, server!');

    // Handle data received from the server
    client.on('data', (data) => {
        console.log('Received from server:', data.toString());
    });

    // Handle server connection closing
    client.on('close', () => {
        console.log('Connection closed');
    });
});

// Handle connection errors
client.on('error', (error) => {
    console.error('Error:', error.message);
});