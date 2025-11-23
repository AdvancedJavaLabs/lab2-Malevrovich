cmake_minimum_required(VERSION 3.12)
project(itmo-parallel-lab2-consumer CXX)

set(CMAKE_CXX_STANDARD 20) 

find_package(amqpcpp REQUIRED)
find_package(gflags REQUIRED)

add_subdirectory(common)
add_subdirectory(consumer)
add_subdirectory(producer)