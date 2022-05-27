cmake_minimum_required(VERSION 3.10)
project(DRIVING_GAME LANGUAGES CXX)

add_executable(game main.cpp)
target_link_libraries(game SDL2 SDL2main)
