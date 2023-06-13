
cmake_minimum_required(VERSION 3.16)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_library(${CMAKE_PROJECT_NAME}
  ObjectDetector.cpp 
  Config.cpp 
  Common.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
  tensorflow-lite ${OpenCV_LIBS}
)