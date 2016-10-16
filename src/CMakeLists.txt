cmake_minimum_required(VERSION 3.6)

project(bdc)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.c utils/bHashTable.c utils/bHashTable.h symbolTable/symbolTable.c symbolTable/symbolTable.h)

add_executable(bdc ${SOURCE_FILES})