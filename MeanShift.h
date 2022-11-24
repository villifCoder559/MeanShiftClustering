#ifndef _MeanShift_
#define _MeanShift_
#include "Kernel.h"
#include "KernelFlat.h"
#include "KernelGaussian.h"
#include "Point.h"
#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <omp.h>
#include <vector>
#include "ClusterCentroids.h"

enum type_kernel { flat = 0, gaussian = 1 };
class MeanShift {
public:
  MeanShift(float bandwidth, type_kernel kernel, short int max_iterations = 50);
  ~MeanShift();
  std::vector<Point> fit(std::vector<Point> points);
  /*Find the closest cluster to point p*/
  unsigned short int predict(Point p);
  type_kernel get_type_kernel();
  /*id n-th corresponds to centroid in position n-th*/
  std::vector<unsigned short int> get_labels();
  float get_bandwidth();

private:
  std::vector<Point> centroids;
  std::vector<unsigned short int> labels;
  float bandwidth;
  Kernel *kernel;
  short int max_iterations;
  unsigned short int find_nearest_centroid(Point &point, std::vector<Point> &centroids);
  std::vector<Point> compute_centroids_parallel(std::vector<Point> &points);
};
#endif