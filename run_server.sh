#!/bin/bash

# Navigate to the server directory
cd server || { echo "Failed to navigate to server directory"; exit 1; }

# Check if the server executable exists
if [ ! -f "bin/server" ]; then
  echo "Server executable not found. Please build the server first."
  exit 1
fi

# Run the server executable
echo "Starting Server..."
./bin/server
