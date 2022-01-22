cmake_minimum_required(VERSION 3.16.9)
project(HW5)

set(CMAKE_CXX_STANDARD 17)

add_executable(HW5 bp.cpp bp.hpp NonTerminalClasses.h NonTerminalClasses.cpp SymbolTable.h SymbolTable.cpp)