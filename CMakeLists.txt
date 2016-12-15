cmake_minimum_required(VERSION 2.8)

#set(SERVICE "test")

project(${SERVICE})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11")
include_directories(include)
add_executable(VLAD include/server.hpp include/client.hpp);

set(Boost_USE_STATIC_LIBS FALSE) 
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME FALSE) 
add_definitions( -DBOOST_ALL_NO_LIB )
find_package(Boost 1.62.0 COMPONENTS filesystem asio bind thread REQUIRED)
include_directories(${Boost_INCLUDE_DIRS}) 
link_directories(${Boost_LIBRARY_DIRS})

target_link_libraries(VLAD ${Boost_LIBRARIES})

find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

target_link_libraries(VLAD ${CURL_LIBRARY} ${OPENSSL_LIBRARIES})

if(BUILD_TESTS)
    enable_testing()
    file(GLOB ${PROJECT_NAME}_TEST_SOURCES tests/init.cpp)
    add_executable(check ${${PROJECT_NAME}_TEST_SOURCES})
    target_link_libraries(check ${PROJECT_NAME} ${CURL_LIBRARY} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})
    add_test(NAME check COMMAND check "-s" "-r" "compact" "--use-colour" "yes") 
endif()
