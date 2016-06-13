cmake_minimum_required (VERSION 2.8)
project (rdtest)

file(GLOB_RECURSE SOURCE *.cpp *.h)

include_directories(${PROJECT_NAME} .)

add_library(${PROJECT_NAME} ${SOURCE})

