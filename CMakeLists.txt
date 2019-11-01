cmake_minimum_required(VERSION 3.15)
project(SFML_Practice)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_MODULE_PATH}/usr/share/SFML/cmake/Modules/")


link_directories(SFML_DIR)
include_directories(SFML_DIR)

find_package(SFML COMPONENTS system window graphics network audio)

add_executable(SFMLApp main.cpp Game.cpp Game.h Board.cpp Board.h)
target_link_libraries (SFMLApp sfml-graphics sfml-window sfml-system sfml-audio)