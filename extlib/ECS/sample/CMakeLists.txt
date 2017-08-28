set(${PROJECT_NAME2} "testECS")

include(CheckCXXCompilerFlag)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")


include_directories("../lib/include")

link_directories("../bin")
add_executable("testECS" main.cpp)
set_target_properties("testECS" PROPERTIES LINK_FLAGS "-s ALLOW_MEMORY_GROWTH=1")
target_link_libraries("testECS" ECS)
