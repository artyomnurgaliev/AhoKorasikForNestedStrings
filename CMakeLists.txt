cmake_minimum_required(VERSION 3.15)
project(untitled4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -lm -lcrypt -O2 -std=c++17 -pipe -DONLINE_JUDGE -fsanitize=address,undefined,leak -fno-omit-frame-pointer -g -Wall -Wconversion -Wextra -Werror")
add_executable(untitled4 main.cpp)