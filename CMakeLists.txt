
cmake_minimum_required(VERSION 3.16)
project(tflite_object_detection C CXX)

set(OpenCV_STATIC ON)
find_package( OpenCV REQUIRED )

set(TENSORFLOW_SOURCE_DIR "" CACHE PATH
  "Directory that contains the TensorFlow project"
)

if(NOT TENSORFLOW_SOURCE_DIR)
  get_filename_component(TENSORFLOW_SOURCE_DIR
    "${CMAKE_CURRENT_LIST_DIR}/thirdparty/tensorflow"
    ABSOLUTE
  )
endif()


message(TENSORFLOW_SOURCE_DIR="${TENSORFLOW_SOURCE_DIR}")
message(CMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}")

add_subdirectory(
  "${TENSORFLOW_SOURCE_DIR}/tensorflow/lite"
  "${CMAKE_CURRENT_BINARY_DIR}/tensorflow-lite"
  EXCLUDE_FROM_ALL
)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_subdirectory(src)
add_subdirectory(examples)

install (TARGETS ${CMAKE_PROJECT_NAME} DESTINATION lib)
file(GLOB headers "include/*.h")
install (FILES ${headers} DESTINATION include)