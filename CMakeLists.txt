cmake_minimum_required(VERSION 3.10)

project(ad8400_ad8402_ad8403 VERSION 0.1.0)
set(CMAKE_C_STANDARD 90)

include_directories(include)
# file(GLOB_RECURSE SRC_FILES src/*.c)
# add_library(ad8400_ad8402_ad8403 STATIC ${SRC_FILES})

add_executable(test test/test.c)
# target_link_libraries(test ad8400_ad8402_ad8403)
