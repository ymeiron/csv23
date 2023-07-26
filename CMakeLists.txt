cmake_minimum_required(VERSION 3.15)

project(csv-reader)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic-errors -g -fsanitize=address")

configure_file(data/iris.csv iris.csv COPYONLY)

find_package(GTest REQUIRED)
enable_testing()
add_executable(${PROJECT_NAME}_test test/test.cpp)
target_link_libraries(${PROJECT_NAME}_test GTest::gtest_main)
target_include_directories(${PROJECT_NAME}_test PUBLIC ${PROJECT_SOURCE_DIR})
add_test(test ${PROJECT_NAME}_test)

