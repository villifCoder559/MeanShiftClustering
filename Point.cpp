#include "Point.h"

Point::Point(std::vector<float> point) {
  for (int i = 0; i < point.size(); i++) coords[i] = point[i];
}
void Point::print() {
  std::cout << "[";
  for (int i = 0; i < coords.size(); i++) {
    std::cout << coords[i];
    if (i != coords.size() - 1) std::cout << ", ";
  }
  std::cout << "]";
  std::cout << "," << std::endl;
}

void Point::add_coord(float value) { this->coords.push_back(value); }

float Point::eucledian_distance(Point x1, Point x2) {
  float distance = 0;
  if (x1.coords.size() == x2.coords.size()) {
    for (int i = 0; i < x1.coords.size(); i++)
      distance += pow((x1.coords[i] - x2.coords[i]), 2);
  }
  return sqrt(distance);
}
bool Point::operator==(Point &obj) {
  bool equal = true;
  if (obj.coords.size() == coords.size()) {
    for (int i = 0; i < coords.size() && equal; i++)
      if (!approximatelyEqual(coords[i], obj.coords[i],std::numeric_limits<float>::epsilon())) 
        equal = false;
  } else
    return false;
  return equal;
}
bool Point::operator!=(Point &obj) { return !operator==(obj); }

bool Point::approximatelyEqual(float a, float b, float epsilon)
{
    return fabs(a - b) <= epsilon;
}