#ifndef _Point_
#define _Point_
#include <iostream>
#include <math.h>
#include <vector>

class Point {
public:
  float calc_L2_norm(Point &x1, Point &x2);
  // int id;
  int weight = 0;
  Point(std::vector<float> point);
  Point(int dim);
  void add_coord(int pos, float value);
  void print();
  bool operator==(const Point &obj) const;
  bool operator!=(const Point &obj) const;
  std::vector<float> get_coords();
  Point operator+(const Point &obj) const;
  Point operator-(const Point &obj) const;
  Point &operator=(const Point &obj);
  Point &operator+=(const Point &obj);
  Point &operator-=(const Point &obj);
  Point operator*(const float &scalar) const;
  Point operator/(const float &scalar) const;
  inline Point &sum_product(const Point &p, const float &weight) {
    const int dim = coords.size();
#pragma omp unroll full
    for (int i = 0; i < dim; i++)
      coords[i] += p.coords[i] * weight;
    return *this;
  }
  inline Point &division(const float &weight) {
    const int dim = coords.size();
#pragma omp unroll full
    for (int i = 0; i < dim; i++)
      coords[i] = coords[i] / weight;
    return *this;
  }
  static inline float calc_L2_norm_approx(Point &x1, Point &x2) {
    float distance = 0;
    int dim = x1.coords.size();
#pragma omp unroll full
    for (int i = 0; i < dim; i++) {
      distance += (x1.coords[i] - x2.coords[i]) * (x1.coords[i] - x2.coords[i]);
    }
    return (distance);
  }
  inline Point &set_zeros() { 
    coords = std::vector<float>(coords.size()); 
    return *this;
  }

private:
  std::vector<float> coords;
  bool approximatelyEqual(float a, float b, float epsilon) const;
};

#endif