cmake_minimum_required (VERSION 2.8)
project (unittest)

file(GLOB_RECURSE SOURCE *.cpp *.h)

include_directories(${PROJECT_NAME} . ../rdtest)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} rdtest)
