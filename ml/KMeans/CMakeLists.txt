cmake_minimum_required(VERSION 3.10)
project(Kmeans)

set(CMAKE_CXX_STANDARD 11)

add_executable(Kmeans main.cpp KMeans.cpp KMeans.h)