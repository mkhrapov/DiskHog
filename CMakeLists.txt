cmake_minimum_required(VERSION 3.31)
project(DiskHog)

set(CMAKE_CXX_STANDARD 20)

add_executable(DiskHog main.cpp
        DiskHog.h
        DiskHog.cpp)
