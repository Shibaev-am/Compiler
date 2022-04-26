cmake_minimum_required(VERSION 3.19)
project(Compiler)

set(CMAKE_CXX_STANDARD 20)

add_executable(Compiler main.cpp lexer.cpp parser.cpp cmake-build-debug/semantics.cpp)