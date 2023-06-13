#
# Copyright 2020 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


# Builds the main Tensorflow Lite example.

cmake_minimum_required(VERSION 3.16)
project(minimal C CXX)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(minimal main.cpp)

target_link_libraries(minimal ${CMAKE_PROJECT_NAME})

install(TARGETS minimal DESTINATION examples/minimal/)
install(FILES config.txt DESTINATION examples/minimal/)