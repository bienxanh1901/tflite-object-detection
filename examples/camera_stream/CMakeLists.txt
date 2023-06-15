cmake_minimum_required(VERSION 3.16)
project(camera_stream C CXX)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(camera_stream main.cpp)

target_link_libraries(camera_stream ${CMAKE_PROJECT_NAME})

install(TARGETS camera_stream DESTINATION examples/camera_stream/)
install(FILES config.txt DESTINATION examples/camera_stream/)