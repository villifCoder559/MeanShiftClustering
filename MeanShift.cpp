#include "MeanShift.h"

MeanShift::MeanShift(float bandwidth, type_kernel kernel) {
  this->bandwidth = bandwidth;
  // this->points = points;
  if (kernel == gaussian)
    this->kernel = new KernelGaussian();
  else
    this->kernel = new KernelFlat();
}

std::vector<Point> MeanShift::fit(std::vector<Point> points) {
  int dim = points[0].coords.size();
  std::vector<Point> mean_vectors;
  for (int i = 0; i < points.size(); i++) {
    mean_vectors.push_back(Point(dim));
  }
  float highest_norm = 6;
  int count = 0;
  do {
    for (int i = 0; i < points.size(); i++) {
      mean_vectors[i] = compute_new_mean(points, points[i]);
      float norm = Point::eucledian_distance(points[i], mean_vectors[i]);
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
  } while (highest_norm > 0.01);
  centroids = get_centroids(points);
  return centroids;
}
Point MeanShift::compute_new_mean(std::vector<Point> &points, Point p) {
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
  if (scale_factor != 0)
    mean = mean / scale_factor;
  return mean;
}
std::vector<Point> MeanShift::get_centroids(std::vector<Point> &points) {
  std::vector<Point> centroids;
  centroids.push_back(points[0]);
  for (int i = 1; i < points.size(); i++) {
    std::vector<float> best_match = find_nearest_centroid(points[i], centroids);
    if (best_match[1] > 3)
      centroids.push_back(points[i]);
  }
  return centroids;
}
std::vector<float> MeanShift::find_nearest_centroid(
    Point &point, std::vector<Point> &centroids) {
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
int MeanShift::predict(Point p) {
  return (int)find_nearest_centroid(p, centroids)[0];
}
type_kernel MeanShift::get_type_kernel() {
  if (typeid(*kernel) == typeid(KernelGaussian))
    return gaussian;
  else
    return flat;
}
MeanShift::~MeanShift() { delete kernel; }