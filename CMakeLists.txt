cmake_minimum_required(VERSION 2.8.7)

project(test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

option(BUILD_SHARED_LIBS "Build shared instead of static library" OFF)
option(BUILD_TESTS "Build tests for webdavclient" ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/ tests/ examples/)
add_library(test STATIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ss.cpp tests/main.cpp)

find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(test ${Boost_LIBRARIES})
endif()

if(UNIX)
    set(DEPENDS_LIBRARIES ${OPENSSL_LIBRARIES} ${CURL_LIBRARY})
elseif(MSVC)
    set(DEPENDS_LIBRARIES ${OPENSSL_LIBRARIES} ${CURL_LIBRARY} ws2_32)
endif()

add_executable(Examples ${CMAKE_CURRENT_SOURCE_DIR}/examples/main.cpp)
target_link_libraries(Examples ${PROJECT_NAME} ${DEPENDS_LIBRARIES})

if(BUILD_TESTS)
    enable_testing()
    file(GLOB ${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
    add_executable(init ${${PROJECT_NAME}_TEST_SOURCES})
    target_link_libraries(init ${PROJECT_NAME} ${DEPENDS_LIBRARIES})
    add_test(NAME init COMMAND init "-s" "-r" "compact" "--use-colour" "yes")
endif()
