#include "Point.h"
Point::Point(int dim) {
  coords.resize(dim);
  // for (int i = 0; i < dim; i++)
  //   coords.push_back(0.0);
}
Point::Point(std::vector<float> point) {
  coords.reserve(point.size());
  for (int i = 0; i < point.size(); i++)
    coords.push_back(point[i]);
}
void Point::print() {
  std::cout << "[";
  for (int i = 0; i < coords.size(); i++) {
    std::cout << coords[i];
    if (i != coords.size() - 1)
      std::cout << ", ";
  }
  std::cout << "]";
  std::cout << "," << std::endl;
}

void Point::add_coord(int pos, float value) {
  if (pos < coords.size())
    this->coords[pos] = value;
}
// void Point::set_zeros() { 
//   coords = std::vector<float>(coords.size());
//   // #pragma omp simd
//   // for(int i=0;i<coords.size();i++)
//   //   coords[i]=0;
//   }

float Point::calc_L2_norm(Point &x1, Point &x2) {
  float distance = 0;
  const int dim = x1.coords.size();
#pragma omp simd reduction(+ : distance)
  for (int i = 0; i < dim; i++) {
    distance += (x1.coords[i] - x2.coords[i])*(x1.coords[i] - x2.coords[i]);
    //  distance+=pow(x1.coords[i]-x2.coords[i],2);
  }
  return sqrt(distance);
}
bool Point::operator==(const Point &obj) const {
  bool equal = true;
  for (int i = 0; i < coords.size() && equal; i++)
    if (!approximatelyEqual(coords[i], obj.coords[i], std::numeric_limits<float>::epsilon()))
      equal = false;
  return equal;
}
bool Point::operator!=(const Point &obj) const { return !operator==(obj); }

bool Point::approximatelyEqual(float a, float b, float epsilon) const { return fabs(a - b) <= epsilon; }
std::vector<float> Point::get_coords() { return coords; }
Point Point::operator+(const Point &obj) const {
  const int dim = coords.size();
  Point new_point = Point(dim);
  for (int i = 0; i < dim; i++)
    new_point.coords[i] = coords[i] + obj.coords[i];
  return new_point;
}

Point &Point::operator+=(const Point &obj) {
  const int dim = coords.size();
  for (int i = 0; i < dim; i++)
    coords[i] += obj.coords[i];
  return *this;
}

Point Point::operator-(const Point &obj) const {
  const int dim = coords.size();
  Point new_point = Point(dim);
  for (int i = 0; i < dim; i++)
    new_point.coords[i] = (coords[i] - obj.coords[i]);
  return new_point;
}

Point &Point::operator-=(const Point &obj) {
  const int dim = coords.size();
  for (int i = 0; i < dim; i++)
    coords[i] -= obj.coords[i];
  return *this;
}
Point &Point::operator=(const Point &obj) {
  coords = obj.coords;
  return *this;
}
Point Point::operator/(const float &scalar) const {
  const int dim = coords.size();
  Point new_point = Point(dim);
#pragma omp simd
  for (int i = 0; i < dim; i++)
    new_point.coords[i] = coords[i] / scalar;
  return new_point;
}
Point Point::operator*(const float &scalar) const {
  const int dim = coords.size();
  Point new_point = Point(dim);
  for (int i = 0; i < dim; i++)
    new_point.coords[i] = coords[i] * scalar;
  return new_point;
}
/*
  mean = mean + points[j] * weight;
*/
// Point &Point::sum_product(const Point &p, const float &weight) {
//   const int dim = coords.size();
//   for (int i = 0; i < dim; i++)
//     coords[i] += p.coords[i] * weight;
//   return *this;
// }
// Point &Point::normalize(const float &weight) {
//   const int dim = coords.size();
//   for (int i = 0; i < dim; i++)
//     coords[i] = coords[i] / weight;
//   return *this;
// }