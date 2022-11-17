#include <omp.h>

#include <fstream>
#include <iostream>

#include "MeanShift.h"
#include "Point.h"
std::vector<Point> read_point_from_file(int);
void print_all_points(std::vector<Point>);

int main() {
  std::vector<Point> points = read_point_from_file(2);
  MeanShift clustering = MeanShift(points, 5, flat);
  std::cout << "Bandwidth: " << clustering.bandwidth << " using Kernel: "
            << (clustering.get_type_kernel() == gaussian ? "Gaussian" : "Flat")
            << std::endl;
  double start = omp_get_wtime();
  auto new_points = clustering.fit();
  double end = omp_get_wtime();
  std::cout << "TIME: " << end - start << std::endl;
  std::cout << "NEW_POINTS" << std::endl;
  print_all_points(new_points);
  return 0;
}
std::vector<Point> read_point_from_file(int dim) {
  std::vector<Point> points;
  std::ifstream file("..\\points.txt");
  std::string line;
  int i = -1;
  int count = 0;
  // points.push_back(Point(3));
  while (std::getline(file, line, ',')) {
    if (count % dim == 0) {
      i += 1;
      points.push_back(Point(dim));
      count=0;
    }
    float value = std::stof(line);
    points[i].add_coord(count,value);
    count++;
  }
  file.close();
  return points;
}

void print_all_points(std::vector<Point> vector) {
  for (int i = 0; i < vector.size(); i++) {
    vector[i].print();
  }
}
