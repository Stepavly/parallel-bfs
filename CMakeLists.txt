cmake_minimum_required(VERSION 3.20)
project(bfs)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenMP REQUIRED)

add_executable(benchmark benchmark.cpp algos.h)
add_executable(stress stress.cpp algos.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -Wextra -Wall -fno-elide-constructors")

target_link_libraries(stress PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(benchmark PRIVATE OpenMP::OpenMP_CXX)
