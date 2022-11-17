#include "Point.h"
Point::Point(int dim){
  // coords.reserve(dim);
  for(int i=0;i<dim;i++)
    coords.push_back(0.0);
}
Point::Point(std::vector<float> point) {
  coords.reserve(point.size());
  for (int i = 0; i < point.size(); i++) coords.push_back(point[i]);
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

void Point::add_coord(int pos,float value) { 
  if(pos<coords.size())
    this->coords[pos]=value;
  }

float Point::eucledian_distance(Point &x1, Point &x2) {
  float distance = 0;
  if (x1.coords.size() == x2.coords.size()) {
    for (int i = 0; i < x1.coords.size(); i++)
      distance += pow((x1.coords[i] - x2.coords[i]), 2);
  }
  return sqrt(distance);
}
bool Point::operator==(const Point &obj) const {
  bool equal = true;
  if (obj.coords.size() == coords.size()) {
    for (int i = 0; i < coords.size() && equal; i++)
      if (!approximatelyEqual(coords[i], obj.coords[i],
                              std::numeric_limits<float>::epsilon()))
        equal = false;
  } else
    return false;
  return equal;
}
bool Point::operator!=(const Point &obj) const { return !operator==(obj); }

bool Point::approximatelyEqual(float a, float b, float epsilon) const {
  return fabs(a - b) <= epsilon;
}
//Add exception different size
Point Point::operator+(const Point &obj) const {
  Point new_point = Point(coords.size());
  if (obj.coords.size() == coords.size()) {
    for (int i = 0; i < coords.size(); i++)
      new_point.coords[i]=coords[i] + obj.coords[i];
  }
  return new_point;
}
//Add exception ...
Point& Point::operator+=(const Point &obj) { 
  if (obj.coords.size() == coords.size()) {
    for (int i = 0; i < coords.size(); i++)
      coords[i]+=obj.coords[i];
  }
  return *this;
}
//Add exception
Point Point::operator-(const Point &obj) const {
  Point new_point = Point(coords.size());
  if (obj.coords.size() == coords.size()) {
    for (int i = 0; i < coords.size(); i++)
      new_point.coords[i]=(coords[i] - obj.coords[i]);
  }
  return new_point;
}
//Add exception ...
Point& Point::operator-=(const Point &obj) { 
  if (obj.coords.size() == coords.size()) {
    for (int i = 0; i < coords.size(); i++)
      coords[i]-=obj.coords[i];
  }
  return *this;
}
Point& Point::operator=(const Point &obj){
  coords=obj.coords;
  return *this;
}
Point Point::operator/(const float &scalar) const{
  Point new_point=Point(coords);
  for(int i=0;i<coords.size();i++)
    new_point.coords[i]=coords[i]/scalar;
  return new_point;
}
Point Point::operator*(const float &scalar) const{
  Point new_point = Point(coords);
  for (int i = 0; i < coords.size(); i++)
      new_point.coords[i]=coords[i]*scalar;
  return new_point;
}
