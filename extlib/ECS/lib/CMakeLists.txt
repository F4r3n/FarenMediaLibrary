cmake_minimum_required (VERSION 2.8.11)
project (ECS CXX)
include(CheckCXXCompilerFlag)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("include")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

include_directories(${CMAKE_SOURCE_DIR}/extlib/json/include)
file(GLOB SOURCES "src/*.cpp")

add_library(ECS STATIC ${SOURCES} ${HEADERS})



