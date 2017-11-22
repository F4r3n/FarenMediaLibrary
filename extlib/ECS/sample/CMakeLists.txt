set(${PROJECT_NAME2} "testECS")

include(CheckCXXCompilerFlag)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")


include_directories("../lib/include")
include_directories("../extlib/json/include")

link_directories("../bin")
add_executable("testECS" main.cpp)

target_link_libraries("testECS" ECS)
