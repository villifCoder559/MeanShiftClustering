#include "MeanShift.h"

MeanShift::MeanShift(float bandwidth, type_kernel kernel, short int max_iterations) {
  this->bandwidth = bandwidth * bandwidth;
  this->max_iterations = max_iterations;
  if (kernel == gaussian)
    this->kernel = new KernelGaussian();
  else
    this->kernel = new KernelFlat();
}
float MeanShift::get_bandwidth() { return sqrt(bandwidth); }
struct dbl_int {
  double val;
  int idx;
};

std::vector<Point> MeanShift::fit(std::vector<Point> points) {
  if (points.size() == 0)
    throw std::invalid_argument("points vector empty");
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
#pragma omp parallel shared(points, mean_vectors, scale_factor) firstprivate(tot)
    {
#pragma omp declare reduction(+ : Point : omp_out += omp_in) initializer(omp_priv = omp_orig)
#pragma omp for reduction(+ : scale_factor, mean)
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
#pragma omp for reduction(max : highest_norm)
      for (int i = 0; i < tot; i++) {
        float norm = (Point::calc_L2_norm_approx(points[i], mean_vectors[i]));
        highest_norm = norm > highest_norm ? norm : highest_norm;
        points[i] = mean_vectors[i];
      }
    }
    iterations++;
    std::cout << "Iterations " << iterations << " completed" << std::endl;
  } while (highest_norm > threshold && iterations < max_iterations);
  centroids = compute_centroids_parallel(points);
  return centroids;
}

std::vector<Point> MeanShift::compute_centroids_parallel(std::vector<Point> &points) {
  ClusterCentroids list_centroid = ClusterCentroids(points[0], bandwidth);
  int tot = points.size();
  int size = 0;
  unsigned short int closest_index = 0;
#pragma omp declare reduction(+ : ClusterCentroids : omp_out += omp_in) initializer(omp_priv = omp_orig)
#pragma omp parallel shared(points, labels, list_centroid) firstprivate(tot)
  {
#pragma omp for reduction(+ : list_centroid)
    for (int i = 0; i < tot; i++)
      list_centroid += points[i];
#pragma omp for firstprivate(list_centroid, bandwidth, closest_index)
    for (int i = 0; i < tot; i++) {
      labels[i] = find_nearest_centroid(points[i], list_centroid.centroids);
    }
  }
  return list_centroid.centroids;
}
unsigned short int MeanShift::find_nearest_centroid(Point &point, std::vector<Point> &centroids) {
  unsigned short int closest_index = 0;
  float min_distance = Point::calc_L2_norm_approx(point, centroids[closest_index]);
  for (int j = 1; j < centroids.size(); j++) {
    float dist = Point::calc_L2_norm_approx(point, centroids[j]);
    if (dist <= min_distance) {
      closest_index = j;
      min_distance = dist;
    }
  }
  return closest_index;
}
unsigned short int MeanShift::predict(Point p) { return find_nearest_centroid(p, centroids); }
type_kernel MeanShift::get_type_kernel() {
  if (typeid(*kernel) == typeid(KernelGaussian))
    return gaussian;
  else
    return flat;
}
std::vector<unsigned short int> MeanShift::get_labels() { return labels; }
MeanShift::~MeanShift() { delete kernel; }