#include <omp.h>

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include "MeanShiftSequential.h"
#include "MeanShiftParallel.h"
#include "Point.h"

std::vector<Point> read_point_from_file();
void print_all_points(std::vector<Point>);
void print_labels(std::vector<unsigned short int>);

int main() {
  std::vector<Point> points = read_point_from_file();
  std::cout<<points.size()<<std::endl;
  MeanShift *clustering = new MeanShiftParallel(5, flat);
  std::cout << "Bandwidth: " << clustering->get_bandwidth() << " using " << (clustering->get_type_kernel() == gaussian ? "gaussian" : "flat") << " kernel " << std::endl;
  float distance = 0;
  int count = 0;
  int tot=points.size();
  std::cout<<tot<<std::endl;
  double start = omp_get_wtime();
  auto centroids = clustering->fit(points);
  double end = omp_get_wtime();
  std::cout << "TIME: " << end - start << std::endl;
  print_all_points(centroids);
  // print_labels(clustering.get_labels());
  return 0;
}

std::vector<Point> read_point_from_file() {
  std::vector<Point> points;
  std::ifstream file("..\\points.txt");
  std::string line;
  int i = -1;
  int count = 0;
  std::getline(file, line, ',');
  int dim = std::stoi(line);
  while (std::getline(file, line, ',')) {
    if (count % dim == 0) {
      i += 1;
      points.push_back(Point(dim));
      count = 0;
    }
    float value = std::stof(line);
    points[i].add_coord(count, value);
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
void print_labels(std::vector<unsigned short int> labels) {
  std::cout << "[";
  for (int i = 0; i < labels.size(); i++) {
    std::cout << labels[i];
    if (i != labels.size() - 1)
      std::cout << ",";
  }
  std::cout << "]";
}
