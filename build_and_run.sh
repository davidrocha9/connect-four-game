#!/bin/bash

# Define project root directory and build directory
PROJECT_ROOT=$(pwd)
BUILD_DIR="$PROJECT_ROOT/build"

# Create the build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir "$BUILD_DIR"
fi

# Navigate to the build directory
cd "$BUILD_DIR"

# Configure the project with CMake
echo "Configuring the project with CMake..."
cmake ..

# Build the project
echo "Building the project..."
cmake --build . -- VERBOSE=1

# Check if the build was successful
if [ $? -ne 0 ]; then
    echo "Build failed. Exiting."
    exit 1
fi

# Run the executable
echo "Running the game..."
./Connect4Game

# Return to the project root directory
cd "$PROJECT_ROOT"
