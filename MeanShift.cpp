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

const dbl_int &max(const dbl_int &a, const dbl_int &b) { return a.val > b.val ? a : b; }

std::vector<Point> MeanShift::fit(std::vector<Point> points) {
  if (points.size() == 0)
    throw std::invalid_argument("points vector empty");
  int dim = points[0].get_coords().size();
  std::vector<Point> mean_vectors;
  float highest_norm;
  int count = 0;
  int tot = points.size();
  mean_vectors.resize(points.size(), Point(dim));
  do {
    highest_norm = -1;
    double start = omp_get_wtime();
#pragma omp parallel for reduction(max : highest_norm) firstprivate(tot) shared(points, mean_vectors)
    for (int i = 0; i < tot; i++) {
      mean_vectors[i] = compute_new_mean(points, points[i]);
      float norm = (Point::calc_L2_norm_approx(points[i], mean_vectors[i]));
      highest_norm = norm > highest_norm ? norm : highest_norm;
    }
    double end = omp_get_wtime();
    std::cout << "time mean -> " << end - start << std::endl;
    // std::cout << "norm -> " << highest_norm << std::endl;
    for (int i = 0; i < points.size(); i++)
      points[i] = mean_vectors[i];
    count++;
  } while (highest_norm > 0.001 && count < max_iterations);
  double start = omp_get_wtime();
  centroids = compute_centroids(points);
  double end = omp_get_wtime();
  std::cout << "time centroids -> " << end - start << std::endl;
  return centroids;
}
Point MeanShift::compute_new_mean(std::vector<Point> &points, Point p) {
  Point mean = Point(p.get_coords().size());
  float scale_factor = 0;
  int tot_el = points.size();
#pragma omp declare reduction(+ : Point : omp_out += omp_in) initializer(omp_priv = omp_orig)
#pragma omp parallel for reduction(+ : scale_factor, mean) firstprivate(p, tot_el) shared(points)
  for (int i = 0; i < tot_el; i++) {
    float dist = Point::calc_L2_norm_approx(p, points[i]);
    float weight = kernel->compute(dist, bandwidth);
    mean = mean + points[i] * weight;
    scale_factor += weight;
  }
  return mean / scale_factor;
}
// Point MeanShift::compute_new_mean(std::vector<Point> &points, float i, std::vector<float> &result) {
//   Point mean = Point(points[0].get_coords().size());
//   float scale_factor = 0;
//   int tot_el = points.size();
//   int n = tot_el;
//   int j = i + 1;
//   int id;
//   // #pragma omp declare reduction(+ : Point : omp_out += omp_in) initializer(omp_priv = omp_orig)
//   // for (int i = 0; i < n; i++)
//   // #pragma omp parallel for firstprivate(tot_el, n) shared(points, result) reduction(+ : scale_factor, mean)
//   for (int k = 0; k < n; k++) {
//     // float dist = Point::eucledian_distance(p, points[i]);
//     if (k != i) {
//       id = (n * (n - 1) / 2) - (n - i) * ((n - i) - 1) / 2 + j - i - 1;
//       float weight = kernel->compute(result[id], bandwidth);
//       mean = mean + points[k] * weight;
//       scale_factor += weight;
//       j++;
//     }
//   }
//   return mean / scale_factor;
// }
// std::vector<Point> MeanShift::fit2(std::vector<Point> points) {
//   if (points.size() == 0)
//     throw std::invalid_argument("points vector empty");
//   int dim = points[0].get_coords().size();
//   std::vector<Point> mean_vectors;
//   float highest_norm = 6;
//   int count = 0;
//   mean_vectors.resize(points.size(), Point(dim));
//   std::vector<float> distances;
//   distances.resize(12497500);
//   while (highest_norm > 0.01 && count < max_iterations) {
//     double start = omp_get_wtime();
//     compute_all_distances(points, 12497500, distances);
//     for (int i = 0; i < points.size(); i++) {
//       mean_vectors[i] = compute_new_mean(points, i, distances);
//       float norm = Point::eucledian_distance(points[i], mean_vectors[i]);
//       if (i == 0)
//         highest_norm = norm;
//       else if (norm > highest_norm)
//         highest_norm = norm;
//     }
//     for (int i = 0; i < points.size(); i++)
//       points[i] = mean_vectors[i];
//     double end = omp_get_wtime();
//     std::cout << "time mean_norm -> " << end - start << std::endl;
//     count++;
//   }
//   // double start = omp_get_wtime();
//   centroids = compute_centroids(points);
//   // double end = omp_get_wtime();
//   // std::cout << "time centroids -> " << end-start << std::endl;
//   return centroids;
// }

// void MeanShift::compute_all_distances(std::vector<Point> &ps, int length, std::vector<float> &distances) {
//   int count = 0;
//   int n = ps.size() - 1;
//   for (int i = 0; i < ps.size(); i++) {
// #pragma parallel for private(i)
//     for (int j = i; j < ps.size(); j++)
//       distances[(n * (n - 1) / 2) - (n - i) * ((n - i) - 1) / 2 + j - i - 1] = Point::eucledian_distance(ps[i], ps[j]);
//   }
// }
std::vector<Point> MeanShift::compute_centroids(std::vector<Point> &points) {
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
std::vector<float> MeanShift::find_nearest_centroid(Point &point, std::vector<Point> &centroids) {
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
unsigned short int MeanShift::predict(Point p) { return (int)find_nearest_centroid(p, centroids)[0]; }
type_kernel MeanShift::get_type_kernel() {
  if (typeid(*kernel) == typeid(KernelGaussian))
    return gaussian;
  else
    return flat;
}
std::vector<unsigned short int> MeanShift::get_labels() { return labels; }
MeanShift::~MeanShift() { delete kernel; }