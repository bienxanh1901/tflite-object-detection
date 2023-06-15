cmake_minimum_required(VERSION 3.16)
project(minimal C CXX)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(minimal main.cpp)

target_link_libraries(minimal ${CMAKE_PROJECT_NAME})

install(TARGETS minimal DESTINATION examples/minimal/)
install(FILES config.txt DESTINATION examples/minimal/)