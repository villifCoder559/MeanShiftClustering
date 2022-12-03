#include "MeanShift.h"

MeanShift::MeanShift(float bandwidth, short int max_iterations) {
  this->bandwidth = bandwidth * bandwidth;
  this->max_iterations = max_iterations;
}

MeanShift::~MeanShift() {}

float MeanShift::get_bandwidth() { return sqrt(bandwidth); }
std::vector<unsigned short int> MeanShift::get_labels() { return labels; }