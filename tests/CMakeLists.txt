cmake_minimum_required(VERSION 2.7.8)
project(${CMAKE_PROJECT_NAME})
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/fixtures $<TARGET_FILE_DIR:${PROJECT_NAME}>)
