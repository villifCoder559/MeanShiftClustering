cmake_minimum_required(VERSION 3.0.0)
project(MeanShift VERSION 0.1.0)

add_executable(MeanShift main.cpp MeanShift.h MeanShift.cpp Point.h Point.cpp GaussianApprox.h GaussianApprox.cpp KernelDensityEstimation.h KernelDensityEstimation.cpp)

