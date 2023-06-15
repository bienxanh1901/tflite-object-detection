
cmake_minimum_required(VERSION 3.16)

project(examples C CXX)

add_subdirectory(minimal)
add_subdirectory(camera_stream)

install(DIRECTORY models DESTINATION examples)
install(DIRECTORY demo_data DESTINATION examples)