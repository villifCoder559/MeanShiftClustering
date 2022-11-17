#ifndef _MeanShift_
#define _MeanShift_
#include <math.h>

#include <map>
#include <vector>

#include "GaussianApprox.h"
#include "KernelDensityEstimation.h"
#include "Point.h"

class MeanShift {
 public:
  std::vector<Point> points;
  float bandwidth;
  MeanShift(std::vector<Point> points, float bandwidth);
  std::vector<Point> fit();

 private:
  std::vector<float> compute_shift_vector(Point p);
  float compute_norm_L2(std::vector<float> vector);
  std::vector<Point> get_centroids();
  std::vector<float> find_nearest_centroid(Point &point,std::vector<Point> centroids);
};
#endif