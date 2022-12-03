#ifndef _CENTROIDS_
#define _CENTROIDS_

#include "Point.h"
#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <omp.h>
#include <vector>

class ClusterCentroids {
 public:
  std::vector<Point> centroids;
  float bandwidth;
  ClusterCentroids();
  ClusterCentroids(float bandwidth);
  ClusterCentroids(Point p,float bandwidth);
  void add_element(Point &p);
  ~ClusterCentroids(){};
  ClusterCentroids& operator=(ClusterCentroids &obj);
  ClusterCentroids& operator+=(ClusterCentroids &obj);
  ClusterCentroids& operator+=(Point &obj);
  ClusterCentroids operator+(ClusterCentroids &obj);
  ClusterCentroids& operator+(Point &obj);
};
#endif