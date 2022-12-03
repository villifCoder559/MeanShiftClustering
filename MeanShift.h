#ifndef _MeanShift_
#define _MeanShift_
#include "ClusterCentroids.h"
#include <limits>
#include "Point.h"
#include <iostream>
#include <omp.h>
#include <vector>
enum type_version { sequential = 0, parallel = 1 };
class MeanShift {
public:
  virtual ~MeanShift() = 0;
  MeanShift(float bandwidth, short int max_iterations = 50);
  virtual std::vector<Point> fit(std::vector<Point> points, int n_threads = 1) = 0;
  virtual unsigned short int predict(Point p) = 0;
  std::vector<unsigned short int> get_labels();
  float get_bandwidth();
  virtual type_version get_version() = 0;
protected:
  std::vector<Point> centroids;
  std::vector<unsigned short int> labels;
  float bandwidth;
  short int max_iterations;
};
#endif