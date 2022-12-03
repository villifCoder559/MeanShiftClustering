#ifndef _MeanShiftSequential_
#define _MeanShiftSequential_

#include "MeanShift.h"

class MeanShiftSequential: public MeanShift {
public:
  MeanShiftSequential(float bandwidth, short int max_iterations = 50);
  ~MeanShiftSequential();
  std::vector<Point> fit(std::vector<Point> points,int n_threads);
  /*Find the closest cluster to point p*/
  unsigned short int predict(Point p);
  /*id n-th corresponds to centroid in position n-th*/
  std::vector<unsigned short int> get_labels();
  float get_bandwidth();
  type_version get_version();
private:
  std::vector<float> find_nearest_centroid(Point &point, std::vector<Point> &centroids);
  std::vector<Point> compute_centroids(std::vector<Point> &points);
};
#endif