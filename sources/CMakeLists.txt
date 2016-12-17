cmake_minimum_required(VERSION 2.8.7)
project(${CMAKE_PROJECT_NAME}_lib CXX)
file(GLOB SOURCES *.cpp)
add_library(${PROJECT_NAME} ${SOURCES} ${Boost_LIBRARIES})
