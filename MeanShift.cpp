#include "MeanShift.h"

MeanShift::MeanShift(std::vector<Point> points, float bandwidth) {
  this->bandwidth = bandwidth;
  this->points = points;
}
// update-> return centroids instead of points
std::vector<Point> MeanShift::fit() {
  std::vector<std::vector<float> > shift_vectors(points.size());
  int dim = points[0].coords.size();
  float highest_norm = 6;
  int count = 0;
  do {
    for (int i = 0; i < points.size(); i++) {
      shift_vectors[i] = compute_shift_vector(points[i]);
      float norm = compute_norm_L2(shift_vectors[i]);
      if (i == 0)
        highest_norm = norm;
      else if (norm > highest_norm)
        highest_norm = norm;
    }
    std::cout << "TOP_NORM " << highest_norm << std::endl;
    for (int i = 0; i < points.size(); i++) {
      for (int j = 0; j < dim; j++) {
        points[i].coords[j] -= shift_vectors[i][j];
      }
    }
    count++;
  } while (highest_norm > 0.1);
  return get_centroids();
}
std::vector<float> MeanShift::compute_shift_vector(Point p) {
  std::vector<float> shift(p.coords.size(), 0.0);
  float scale_factor = 0;
  // float constant_value=pow(2*M_PI,p.coords.size());
  for (int dim = 0; dim < p.coords.size(); dim++) {
    for (int i = 0; i < points.size(); i++) {
      if (p != points[i]) {
        float dist = Point::eucledian_distance(p, points[i]);
        float weight = exp((-0.5 * pow(dist,2) / (pow(bandwidth, 2))));
        // std::cout<<weight<<std::endl;
        shift[dim] += points[i].coords[dim] * weight;
        scale_factor += weight;
      }
    }
    shift[dim] = shift[dim] / scale_factor;
    scale_factor = 0.0;
  }
  for (int i = 0; i < p.coords.size(); i++) 
		shift[i] = p.coords[i] - shift[i];
  return shift;
}
float MeanShift::compute_norm_L2(std::vector<float> vector) {
  float norm = 0;
  for (int i = 0; i < vector.size(); i++) {
    norm += pow(vector[i], 2);
  }
  return sqrt(norm);
}
std::vector<Point> MeanShift::get_centroids() {
  std::vector<Point> centroids;
  centroids.push_back(points[0]);
  for (int i = 1; i < points.size(); i++) {
    std::vector<float> best_match = find_nearest_centroid(points[i],centroids);
    if (best_match[1] > 5)
			centroids.push_back(points[i]); 
	}
	return centroids;
}
std::vector<float> MeanShift::find_nearest_centroid(Point &point,std::vector<Point> centroids) {
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