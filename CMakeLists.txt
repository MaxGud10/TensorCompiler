cmake_minimum_required(VERSION 3.20)
project(onnx2dot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party/onnx)


add_library(onnx2dot_includes INTERFACE)
target_include_directories(onnx2dot_includes INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/onnx
)


add_executable(onnx2dot
    src/main.cpp
    src/ir/graph.cpp
    src/third_party/OnnxImporter.cpp
    src/viz/GraphvizDump.cpp
)


target_link_libraries(onnx2dot 
    PRIVATE
    onnx_proto
    ${PROTOBUF_LIBRARIES}
)


target_include_directories(onnx2dot PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/onnx
    ${PROTOBUF_INCLUDE_DIRS}
)

target_compile_features(onnx2dot PRIVATE cxx_std_17)