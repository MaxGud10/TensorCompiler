include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip ## сделать это через conan потом 
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(ir_tests
  test_ir_basic.cpp
)
target_link_libraries(ir_tests PRIVATE ir GTest::gtest_main)
target_include_directories(ir_tests PRIVATE include)

include(GoogleTest)
gtest_discover_tests(ir_tests)