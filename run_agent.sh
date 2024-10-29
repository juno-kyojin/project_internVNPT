#!/bin/bash

# Navigate to the agent directory
cd agent || { echo "Failed to navigate to agent directory"; exit 1; }

# Check if the agent executable exists
if [ ! -f "bin/agent" ]; then
  echo "Agent executable not found. Please build the agent first."
  exit 1
fi

# Run the agent executable
echo "Starting Agent..."
./bin/agent
