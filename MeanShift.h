#ifndef _MeanShift_
#define _MeanShift_
#include <math.h>
#include "Kernel.h"
#include "KernelGaussian.h"
#include "KernelFlat.h"
#include <map>
#include <vector>
#include "Point.h"
#include <iostream>
#include <fstream>

enum type_kernel {flat=0,gaussian=1};

class MeanShift {
 public:
  std::vector<Point> centroids;
  float bandwidth;
  Kernel *kernel;
  MeanShift(float bandwidth,type_kernel kernel);
  ~MeanShift();
  std::vector<Point> fit(std::vector<Point> points);
  int predict(Point p);
  type_kernel get_type_kernel();
  
 private:
  Point compute_new_mean(std::vector<Point> &points,Point p);
  // float compute_norm_L2(Point x1,Point x2);
  std::vector<Point> get_centroids(std::vector<Point> &points);
  std::vector<float> find_nearest_centroid(Point &point,std::vector<Point> &centroids);
};
#endif