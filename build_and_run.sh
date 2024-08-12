#!/bin/bash

# Navigate to the project directory
cd "$(dirname "$0")"

# Remove existing build directory if it exists
if [ -d "build" ]; then
    echo "Removing existing build directory..."
    rm -rf build
fi

# Create a new build directory
mkdir build

# Navigate to the build directory
cd build

# Configure the project with CMake
echo "Configuring the project with CMake..."
cmake ..

# Build the project
echo "Building the project..."
make

# Run the built executable (replace 'connect-4-game' with your actual executable name)
echo "Running the project..."
./Connect4Game
