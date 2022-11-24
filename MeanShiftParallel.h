#ifndef _MeanShiftParallel_
#define _MeanShiftParallel_

#include "MeanShift.h"
class MeanShiftParallel: public MeanShift {
public:
  MeanShiftParallel(float bandwidth, type_kernel kernel, short int max_iterations = 50);
  ~MeanShiftParallel();
  std::vector<Point> fit(std::vector<Point> points);
  /*Find the closest cluster to point p*/
  unsigned short int predict(Point p);
  type_kernel get_type_kernel();
  /*id n-th corresponds to centroid in position n-th*/
  std::vector<unsigned short int> get_labels();
  float get_bandwidth();
  type_version get_version();
private:
  unsigned short int find_nearest_centroid(Point &point, std::vector<Point> &centroids);
  std::vector<Point> compute_centroids(std::vector<Point> &points);
};
#endif