#include "ClusterCentroids.h"

ClusterCentroids::ClusterCentroids() {}
ClusterCentroids::ClusterCentroids(float bandwidth) { this->bandwidth = bandwidth; }
ClusterCentroids::ClusterCentroids(Point p, float bandwidth) {
  centroids.push_back(p);
  this->bandwidth = bandwidth;
}
ClusterCentroids &ClusterCentroids::operator=(ClusterCentroids &obj) {
  centroids = obj.centroids;
  bandwidth = obj.bandwidth;
  return *this;
}
ClusterCentroids &ClusterCentroids::operator+=(Point &obj) {
  int dim = centroids.size();
  bool is_new_element = true;
  for (int i = 0; i < dim && is_new_element; i++) {
    float dist = Point::calc_L2_norm_approx(obj, centroids[i]);
    if (dist < bandwidth)
      is_new_element = false;
  }
  if (is_new_element)
    centroids.push_back(obj);
  return *this;
}
ClusterCentroids &ClusterCentroids::operator+=(ClusterCentroids &obj) {
  float max_dist = 0;
  int dim = obj.centroids.size();
  for (int j = 0; j < dim; j++)
    this->operator+=(obj.centroids[j]);
  return *this;
}
ClusterCentroids ClusterCentroids::operator+(ClusterCentroids &obj) {
  ClusterCentroids new_centroids = ClusterCentroids(bandwidth);
  int dim = obj.centroids.size();
  for (int i = 0; i < dim; i++)
    new_centroids.operator+=(obj.centroids[i]);
  return new_centroids;
}

void ClusterCentroids::add_element(Point &p) { centroids.push_back(p); }