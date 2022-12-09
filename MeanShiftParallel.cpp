#include "MeanShiftParallel.h"

MeanShiftParallel::MeanShiftParallel(float bandwidth, short int max_iterations) : MeanShift(bandwidth, max_iterations) {}
float MeanShiftParallel::get_bandwidth() { return sqrt(bandwidth); }

std::vector<Point> MeanShiftParallel::fit(std::vector<Point> points, int n_threads) {
  if (n_threads <= 0)
    n_threads = omp_get_num_threads();
  int dim = points[0].get_coords().size();
  int tot = points.size();
  int iterations = 0;
  std::vector<Point> mean_vectors;
  float highest_norm = -1;
  float threshold = 0.1 * sqrt(bandwidth);
  Point mean = Point(dim);
  mean_vectors.resize(points.size(), Point(dim));
  labels.resize(tot);
  std::cout << "Start fitting..." << std::endl;
  do {
    highest_norm = -1;
    double start = omp_get_wtime();
#pragma omp parallel shared(points, mean_vectors) firstprivate(tot) 
    {
      float scalar = 0;
      float dist = 0;
      // float weight = 0;
// #pragma omp declare reduction(+ : Point : omp_out += omp_in) initializer(omp_priv = omp_orig)
#pragma omp for firstprivate(mean) 
      for (int i = 0; i < tot; i++) {
        for (int j = 0; j < tot; j++) {
          dist = Point::calc_L2_norm_approx(points[i], points[j]);
          // weight = dist > bandwidth ? 1.0 : 0.0;
          if (dist <= bandwidth) {
            mean.sum_product(points[j], 1.0);
            scalar += 1.0;
          }
        }
        mean.normalize(scalar);
        mean_vectors[i] = mean;
        mean.set_zeros();
        scalar = 0;
      }
#pragma omp for reduction(max : highest_norm)
      for (int i = 0; i < tot; i++) {
        float norm = (Point::calc_L2_norm_approx(points[i], mean_vectors[i]));
        highest_norm = norm > highest_norm ? norm : highest_norm;
        points[i] = mean_vectors[i];
      }
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
std::vector<Point> MeanShiftParallel::compute_centroids(std::vector<Point> &points) {
  ClusterCentroids list_centroid = ClusterCentroids(points[0], bandwidth);
  int tot = points.size();
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
unsigned short int MeanShiftParallel::find_nearest_centroid(Point &point, std::vector<Point> &centroids) {
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

unsigned short int MeanShiftParallel::predict(Point p) { return find_nearest_centroid(p, centroids); }
std::vector<unsigned short int> MeanShiftParallel::get_labels() { return labels; }
type_version MeanShiftParallel::get_version() { return parallel; }
MeanShiftParallel::~MeanShiftParallel() {}