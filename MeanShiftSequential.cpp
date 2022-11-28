#include "MeanShiftSequential.h"

MeanShiftSequential::MeanShiftSequential(float bandwidth, type_kernel kernel, short int max_iterations):MeanShift(bandwidth,kernel,max_iterations){}
float MeanShiftSequential::get_bandwidth() { return sqrt(bandwidth); }

std::vector<Point> MeanShiftSequential::fit(std::vector<Point> points) {
  int dim = points[0].get_coords().size();
  int tot = points.size();
  int iterations = 0;
  std::vector<Point> mean_vectors;
  float highest_norm = -1;
  float scale_factor = 0;
  float threshold = 1e-3 * bandwidth;
  Point mean = Point(dim);
  mean_vectors.resize(points.size(), Point(dim));
  labels.resize(tot);
  std::cout << "Start fitting..." << std::endl;
  do {
    highest_norm = -1;
    double start = omp_get_wtime();
    for (int i = 0; i < tot; i++) {
      for (int j = 0; j < tot; j++) {
        float dist = Point::calc_L2_norm_approx(points[i], points[j]);
        float weight = kernel->compute(dist, bandwidth);
        if (weight != 0) {
          mean.sum_product(points[j], weight);
          scale_factor += weight;
        }
      }
      mean_vectors[i] = mean / scale_factor;
      scale_factor = 0;
      mean.set_zeros();
    }
    for (int i = 0; i < tot; i++) {
      float norm = (Point::calc_L2_norm_approx(points[i], mean_vectors[i]));
      highest_norm = norm > highest_norm ? norm : highest_norm;
      points[i] = mean_vectors[i];
    }
    double end = omp_get_wtime();
    iterations++;
    std::cout << "Iterations " << iterations << " completed in " << end - start << std::endl;
  } while (highest_norm > threshold && iterations < max_iterations);
  double start = omp_get_wtime();
  centroids = compute_centroids(points);
  double end = omp_get_wtime();
  std::cout << "Centroids completed in " << end - start << std::endl;
  return centroids;
}
std::vector<Point> MeanShiftSequential::compute_centroids(std::vector<Point> &points) {
  std::vector<Point> centroids;
  centroids.push_back(points[0]);
  labels.reserve(points.size());
  labels.push_back(0);
  for (int i = 1; i < points.size(); i++) {
    std::vector<float> best_match = find_nearest_centroid(points[i], centroids);
    if (best_match[1] > bandwidth) {
      labels.push_back(centroids.size());
      centroids.push_back(points[i]);
    } else
      labels.push_back(best_match[0]);
  }
  return centroids;
}
std::vector<float> MeanShiftSequential::find_nearest_centroid(Point &point, std::vector<Point> &centroids) {
  std::vector<float> best_match = {0, Point::calc_L2_norm_approx(point, centroids[0])};
  for (int j = 1; j < centroids.size(); j++) {
    float dist = Point::calc_L2_norm_approx(point, centroids[j]);
    if (dist < best_match[1]) {
      best_match[0] = j;
      best_match[1] = dist;
    }
  }
  return best_match;
}

unsigned short int MeanShiftSequential::predict(Point p) { return (unsigned short int)find_nearest_centroid(p, centroids)[0]; }
std::vector<unsigned short int> MeanShiftSequential::get_labels() { return labels; }
type_version MeanShiftSequential::get_version(){ return sequential;}
type_kernel MeanShiftSequential::get_type_kernel(){
  return MeanShift::get_type_kernel();
}
MeanShiftSequential::~MeanShiftSequential() { delete kernel; }
