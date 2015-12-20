cmake_minimum_required(VERSION 3.2)

project(dog_vision C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

find_package(OpenCV REQUIRED)

add_executable(dog_vision
  dog_vision.cc)

target_include_directories(dog_vision
  SYSTEM PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS})

target_link_libraries(dog_vision
  PRIVATE
    ${OpenCV_LIBS})
