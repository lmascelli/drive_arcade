cmake_minimum_required(VERSION 3.10)
project(DRIVING_GAME LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD 20)

set(SOURCES main.cpp
            utility.cpp
            track.cpp)

if(WIN32)
  add_executable(game WIN32 ${SOURCES})
target_include_directories(game PRIVATE include "D:/usr/include")
target_link_directories(game PRIVATE lib "D:/usr/lib")
target_link_libraries(game SDL2main SDL2)
else()
  add_executable(game ${SOURCES})
                    
target_link_libraries(game SDL2main SDL2)
endif()
