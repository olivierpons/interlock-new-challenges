cmake_minimum_required(VERSION 3.21)
project(interlock C)

set(CMAKE_C_STANDARD 99)

add_executable(interlock main.c include/cube.h include/custom_types.h include/piece.h include/block.h include/block.c include/blocks.c)
