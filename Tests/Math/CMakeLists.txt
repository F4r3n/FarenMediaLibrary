cmake_minimum_required(VERSION 3.5)
set(${PROJECT_NAME2} "testMath")

include(CheckCXXCompilerFlag)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories("../../lib/include")
link_directories("../../build/lib")

enable_testing()
set(TEST_NAME TEST_VECTOR)
add_executable(${TEST_NAME} VectorTests.cpp main.cpp)
target_link_libraries(${TEST_NAME} FarenMediaLibrary ${GTEST_LIBRARIES})
add_test(test1 ${TEST_NAME})

enable_testing()
set(TEST_NAME TEST_MATRIX)
add_executable(${TEST_NAME} MatrixTests.cpp main.cpp)
target_link_libraries(${TEST_NAME} FarenMediaLibrary ${GTEST_LIBRARIES})
add_test(test2 ${TEST_NAME})

enable_testing()
set(TEST_NAME TEST_FUNCTIONS)
add_executable(${TEST_NAME} FunctionsTests.cpp main.cpp)
target_link_libraries(${TEST_NAME} FarenMediaLibrary ${GTEST_LIBRARIES})
add_test(test3 ${TEST_NAME})

enable_testing()
set(TEST_NAME TEST_QUADTREE)
add_executable(${TEST_NAME} QuadTreeTests.cpp main.cpp)
target_link_libraries(${TEST_NAME} FarenMediaLibrary ${GTEST_LIBRARIES})
add_test(test4 ${TEST_NAME})
