cmake_minimum_required(VERSION 2.8)
project(MySqlOperator)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -std=c++11")
MESSAGE(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})

include_directories(/usr/include/mysql)
LINK_DIRECTORIES(/usr/lib/x86_64-linux-gnu)

AUX_SOURCE_DIRECTORY(. SRC)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(mysqlOperator ${SRC})
target_link_libraries(mysqlOperator mysqlclient)
