#include <fstream>
#include <iostream>

#include "MeanShift.h"
#include "Point.h"
std::vector<Point> read_point_from_file();
void print_all_points(std::vector<Point>);
// bool approximatelyEqual(float a, float b, float epsilon)
// {
//     return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
// }
int main() {
	std::vector<Point> points=read_point_from_file();
	MeanShift clustering=MeanShift(points,1);
	auto new_points=clustering.fit();
	std::cout<<"NEW_POINTS"<<std::endl;
	print_all_points(new_points);
  return 0;
}
std::vector<Point> read_point_from_file(){
	std::vector<Point> points;
  std::ifstream file("..\\points.txt");
  std::string line;
  int i = 0;
  int count = 1;
	points.push_back(Point());
  while (std::getline(file, line, ',')) {
		float value=std::stof(line);
    points[i].add_coord(value);
    if (count % 3 == 0) {
      i += 1;
      points.push_back(Point());
    }
    count++;
  }
	points.pop_back();
  file.close();
	return points;
}

void print_all_points(std::vector<Point> vector){
	for(int i=0;i<vector.size();i++){
		vector[i].print();
	}
}
