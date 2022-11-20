#ifndef _Point_
#define _Point_
#include <math.h>
#include <iostream>
#include <vector>

class Point {
 public:
  static float eucledian_distance(Point &x1, Point &x2);

 public:
  //int id;
  Point(std::vector<float> point);
	Point(int dim);
  void add_coord(int pos,float value);
  void print();
  bool operator ==(const Point &obj)const;
  bool operator !=(const Point &obj)const;
  std::vector<float> get_coords(){
    return coords;
  }
  Point operator +(const Point &obj)const;
  Point operator -(const Point &obj)const;
  Point& operator =(const Point &obj);
  Point& operator +=(const Point &obj);
  Point& operator -=(const Point &obj);
  Point operator *(const float &scalar) const;
  Point operator /(const float &scalar) const;


  
private:
  std::vector<float> coords;
  bool approximatelyEqual(float a, float b, float epsilon) const;
};
#endif