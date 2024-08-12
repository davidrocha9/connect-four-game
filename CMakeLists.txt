cmake_minimum_required(VERSION 3.10)
project(Connect4Game)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2
find_package(SDL2 REQUIRED)

# Manually specify SDL2_image path
set(SDL2_IMAGE_INCLUDE_DIRS "/opt/homebrew/Cellar/sdl2_image/2.8.2_1/include")
set(SDL2_IMAGE_LIBRARIES "/opt/homebrew/Cellar/sdl2_image/2.8.2_1/lib/libSDL2_image-2.0.0.dylib")

# Manually specify SDL2_ttf path
set(SDL2_TTF_INCLUDE_DIRS "/opt/homebrew/Cellar/sdl2_ttf/2.22.0/include")
set(SDL2_TTF_LIBRARIES "/opt/homebrew/Cellar/sdl2_ttf/2.22.0/lib/libSDL2_ttf-2.0.0.dylib")

# Print the paths for debugging
message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
message(STATUS "SDL2_IMAGE_INCLUDE_DIRS: ${SDL2_IMAGE_INCLUDE_DIRS}")
message(STATUS "SDL2_IMAGE_LIBRARIES: ${SDL2_IMAGE_LIBRARIES}")
message(STATUS "SDL2_TTF_INCLUDE_DIRS: ${SDL2_TTF_INCLUDE_DIRS}")
message(STATUS "SDL2_TTF_LIBRARIES: ${SDL2_TTF_LIBRARIES}")

# Include directories for SDL2, SDL2_image, and SDL2_ttf
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src/game)
include_directories(${CMAKE_SOURCE_DIR}/src/menu)
include_directories(${CMAKE_SOURCE_DIR}/src/board)
include_directories(${CMAKE_SOURCE_DIR}/src/utils)

# Add source files
set(SOURCES
    src/main.cpp
    src/menu/Menu.cpp
    src/board/Board.cpp
    src/game/Game.cpp
    src/utils/Utils.cpp
)

# Add executable
add_executable(Connect4Game ${SOURCES})

# Link SDL2, SDL2_image, and SDL2_ttf libraries
target_link_libraries(Connect4Game ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
