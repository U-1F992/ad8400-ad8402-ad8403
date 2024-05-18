cmake_minimum_required(VERSION 3.10)

project(ad8400_ad8402_ad8403)
set(CMAKE_C_STANDARD 99)

include_directories(include)
add_library(ad8400_ad8402_ad8403 STATIC
            src/ad8400_ad8402_ad8403.c)

add_executable(test test/test.c)
target_link_libraries(test ad8400_ad8402_ad8403)