cmake_minimum_required(VERSION 3.7)
project(inverted_index)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c utils.c utils.h suffixed.c suffixed.h balancedInterleaving.c balancedInterleaving.h filesBlocksOrdered.c filesBlocksOrdered.h)
add_executable(inverted_index ${SOURCE_FILES})