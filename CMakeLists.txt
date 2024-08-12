cmake_minimum_required(VERSION 3.10)
project(Connect4Game)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2
find_package(SDL2 REQUIRED)

# Manually specify SDL2_image path
set(SDL2_IMAGE_INCLUDE_DIRS "/opt/homebrew/Cellar/sdl2_image/2.8.2_1/include")
set(SDL2_IMAGE_LIBRARIES "/opt/homebrew/Cellar/sdl2_image/2.8.2_1/lib/libSDL2_image-2.0.0.dylib")

# Include directories for SDL2 and SDL2_image
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src/game)  # Include directory for Game.h
include_directories(${CMAKE_SOURCE_DIR}/src/board) # Include directory for Board.h

# Add source files
set(SOURCES
    src/main.cpp
    src/board/Board.cpp
    src/game/Game.cpp
)

# Add executable
add_executable(Connect4Game ${SOURCES})

# Link SDL2 and SDL2_image libraries
target_link_libraries(Connect4Game ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
