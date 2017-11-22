set(${PROJECT_NAME2} "testECS")

include(CheckCXXCompilerFlag)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories("../lib/include")
include_directories("../extlib/json/include")
link_directories("../bin")

enable_testing()
set(TEST_NAME TEST_1)
add_executable(${TEST_NAME} allocation_test.cpp main.cpp)
target_link_libraries(${TEST_NAME} ECS ${GTEST_LIBRARIES})
add_test(test1 ${TEST_NAME})

set(TEST_NAME TEST_2)
add_executable(${TEST_NAME} ComponentTest.cpp main.cpp)
target_link_libraries(${TEST_NAME} ECS ${GTEST_LIBRARIES})
add_test(test2 ${TEST_NAME})




