#ifndef _MeanShift_
#define _MeanShift_
#include "ClusterCentroids.h"
#include "Kernel.h"
#include "KernelFlat.h"
#include "KernelGaussian.h"
#include "Point.h"
#include <iostream>
#include <omp.h>
#include <vector>
enum type_kernel { flat = 0, gaussian = 1 };
enum type_version { sequential = 0, parallel = 1 };
class MeanShift {
public:
  virtual ~MeanShift() = 0;
  MeanShift(float bandwidth, type_kernel kernel, short int max_iterations = 50);
  virtual std::vector<Point> fit(std::vector<Point> points) = 0;
  virtual unsigned short int predict(Point p) = 0;
  type_kernel get_type_kernel();
  std::vector<unsigned short int> get_labels();
  float get_bandwidth();
  virtual type_version get_version() = 0;

protected:
  std::vector<Point> centroids;
  std::vector<unsigned short int> labels;
  float bandwidth;
  Kernel *kernel;
  short int max_iterations;
};
#endif