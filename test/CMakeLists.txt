cmake_minimum_required (VERSION 2.8)
project (unittest)

file(GLOB_RECURSE SOURCE *.cpp *.h)

include_directories(${PROJECT_NAME} .)

add_executable(${PROJECT_NAME} ${SOURCE})
