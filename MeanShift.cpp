#include "MeanShift.h"

MeanShift::MeanShift(std::vector<Point> points, float bandwidth,
                     type_kernel kernel) {
  this->bandwidth = bandwidth;
  this->points = points;
  if (kernel == gaussian)
    this->kernel = new KernelGaussian();
  else
    this->kernel = new KernelFlat();
}
// update-> return centroids instead of points
std::vector<Point> MeanShift::fit() {
  int dim = points[0].coords.size();
  std::vector<Point> mean_vectors;
  for(int i=0;i<points.size();i++){
    mean_vectors.push_back(Point(dim));
  }
  float highest_norm = 6;
  int count = 0;
  do {
    for (int i = 0; i < points.size(); i++) {
      mean_vectors[i] = compute_new_mean(points[i]);
      float norm = Point::eucledian_distance(points[i],mean_vectors[i]);
      if (i == 0)
        highest_norm = norm;
      else if (norm > highest_norm)
        highest_norm = norm;
    }
    std::cout << "TOP_NORM " << highest_norm << std::endl;
    for (int i = 0; i < points.size(); i++) {
        points[i] = mean_vectors[i];
    }
    count++;
  } while (highest_norm > 0.001);
  return get_centroids();
}
Point MeanShift::compute_new_mean(Point p) {
  Point mean = Point(p.coords.size());
  float scale_factor = 0;
  for (int i = 0; i < points.size(); i++) {
    if (p != points[i]) {
      float dist = Point::eucledian_distance(p, points[i]);
      float weight = kernel->compute(dist, bandwidth);
      mean += points[i] * weight;
      scale_factor += weight;
    }
  }
  if(scale_factor!=0)
        mean = mean / scale_factor;
  // for (int i = 0; i < p.coords.size(); i++)
    // mean.coords[i] = p.coords[i] - mean.coords[i];
  return mean;
}
std::vector<Point> MeanShift::get_centroids() {
  std::vector<Point> centroids;
  centroids.push_back(points[0]);
  for (int i = 1; i < points.size(); i++) {
    std::vector<float> best_match = find_nearest_centroid(points[i], centroids);
    if (best_match[1] > 2) centroids.push_back(points[i]);
  }
  return centroids;
}
std::vector<float> MeanShift::find_nearest_centroid(Point &point, std::vector<Point> &centroids) {
  std::vector<float> best_match = {-1, 0};
  for (int j = 0; j < centroids.size(); j++) {
    float dist = Point::eucledian_distance(point, centroids[j]);
    if (j == 0) {
      best_match[0] = j;
      best_match[1] = dist;
    } else if (dist < best_match[1]) {
      best_match[0] = j;
      best_match[1] = dist;
    }
  }
  return best_match;
}
type_kernel MeanShift::get_type_kernel() {
  if (typeid(*kernel) == typeid(KernelGaussian))
    return gaussian;
  else
    return flat;
}
MeanShift::~MeanShift() { delete kernel; }