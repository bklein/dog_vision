cmake_minimum_required(VERSION 3.2)

project(dog_vision C CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

#add_executable(dog_vision
#  dog_viz.cc
#  dog_viz_lib.cc)
add_executable(dog_vision
  dog_viz2.cc)

target_include_directories(dog_vision
  SYSTEM PRIVATE
    ${OpenCV_INCLUDE_DIRS}
  PRIVATE
    ${CMAKE_SOURCE_DIR})

target_link_libraries(dog_vision
  PRIVATE
    ${OpenCV_LIBS})
