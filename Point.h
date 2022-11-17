#ifndef _Point_
#define _Point_
#include <math.h>
#include <iostream>
#include <vector>

class Point {
 public:
  static float eucledian_distance(Point x1, Point x2);

 public:
  //int id;
  std::vector<float> coords;
  Point(std::vector<float> point);
	Point(){};
  void add_coord(float value);
  void print();
  bool operator ==(Point &obj);
  bool operator !=(Point &obj);
private:
  bool approximatelyEqual(float a, float b, float epsilon);
};
#endif