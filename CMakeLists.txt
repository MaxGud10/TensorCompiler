cmake_minimum_required(VERSION 3.16)
project(TensorCompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ir
  src/graph.cpp
)
target_include_directories(ir PUBLIC include)

add_executable(onnx2ir
  src/main.cpp
)
target_link_libraries(onnx2ir PRIVATE ir)

enable_testing()
add_subdirectory(tests)