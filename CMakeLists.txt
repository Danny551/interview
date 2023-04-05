cmake_minimum_required(VERSION 3.5)

project(interview LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(circular_queue
  circular_queue.cpp
)

add_executable(mystring
  mystring.cpp
)

add_executable(singleton
  singleton.cpp
)

