cmake_minimum_required(VERSION 2.8)

#set(SERVICE "WebDav")

project(${SERVICE}Client)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.62.0 COMPONENTS system program_options thread filesystem
  date_time chrono timer regex serialization REQUIRED)


option(BUILD_SHARED_LIBS "Build shared instead of static library" OFF)
option(BUILD_TESTS "Build tests for webdavclient" OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)

add_library(${PROJECT_NAME} STATIC ${CMAKE_CURRENT_SOURCE_DIR}/sources/client.cpp)

find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

if(UNIX)
set(DEPENDS_LIBRARIES ${OPENSSL_LIBRARIES} ${CURL_LIBRARY})
elseif(MSVC)
set(DEPENDS_LIBRARIES ${OPENSSL_LIBRARIES} ${CURL_LIBRARY} ws2_32)
endif()
include_directories(${Boost_INCLUDE_DIRS})
add_executable(WW ${CMAKE_CURRENT_SOURCE_DIR}/include/ww.cpp)
add_executable(SS ${CMAKE_CURRENT_SOURCE_DIR}/include/ss.cpp)
target_link_libraries(SS ${Boost_LIBRARIES})
target_link_libraries(WW ${PROJECT_NAME} ${DEPENDS_LIBRARIES} pthread)

if(BUILD_TESTS)
    enable_testing()
    file(GLOB ${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
    add_executable(check ${${PROJECT_NAME}_TEST_SOURCES})
    target_link_libraries(check ${PROJECT_NAME} ${DEPENDS_LIBRARIES})
    add_test(NAME check COMMAND check "-s" "-r" "compact" "--use-colour" "yes") 
endif()
