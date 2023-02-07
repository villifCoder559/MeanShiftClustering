#include <omp.h>

#include "MeanShiftParallel.h"
#include "MeanShiftSequential.h"
#include "Point.h"
#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>

std::vector<Point> read_points_from_file(std::string);
void print_all_points(std::vector<Point>);
void print_labels(std::vector<unsigned short int>);
void test(std::string, std::vector<int>, int);
int main() {
  std::vector<int> n_points = {25000};
  std::vector<int> n_centroids = {5, 10, 20, 50, 100};
  std::vector<int> n_dims = {2, 3, 4, 5};
  test("points", n_points,20);
  // test("centroids", n_centroids);
  // test("dims", n_dims);
  // std::getchar();
  // print_all_points(centroids);
  //  print_labels(clustering.get_labels());
  return 0;
}
void test(std::string test_name, std::vector<int> arr, int n_threads) {
  for (int i = 0; i < arr.size(); i++) {
    std::vector<Point> points = read_points_from_file(test_name + "_" + std::to_string(arr[i]) + ".txt");
    MeanShift *clustering = new MeanShiftParallel(5);
    std::cout << test_name + "_" + std::to_string(arr[i]) + ".txt" << std::endl;
    std::cout << (clustering->get_version() ? "Parallel" : "Sequential") << " Mean Shift" << std::endl;
    std::cout << "Bandwidth: " << clustering->get_bandwidth() << " using "
              << "flat kernel " << std::endl;
    double start = omp_get_wtime();
    auto centroids = clustering->fit(points, n_threads);
    double end = omp_get_wtime();
    std::cout << "TIME: " << end - start << std::endl;
    std::cout << "" << std::endl;
    print_all_points(centroids);
  }
}
std::vector<Point> read_points_from_file(std::string name) {
  std::vector<Point> points;
  std::ifstream file("..\\" + name);
  std::string line;
  std::getline(file, line, ',');
  int i = -1;
  int count = 0;
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
