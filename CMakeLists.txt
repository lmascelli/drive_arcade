cmake_minimum_required(VERSION 3.10)
project(DRIVING_GAME LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

add_executable(game main.cpp)
target_include_directories(game PRIVATE include)
target_link_directories(game PRIVATE lib)
target_link_libraries(game SDL2 SDL2main)
