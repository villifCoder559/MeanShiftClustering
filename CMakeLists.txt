cmake_minimum_required(VERSION 3.0.0)
project(MeanShift VERSION 0.1.0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(MeanShift main.cpp MeanShift.h MeanShift.cpp Point.h Point.cpp Kernel.h Kernel.cpp KernelFlat.h KernelFlat.cpp KernelGaussian.h KernelGaussian.cpp)

