#include "MeanShift.h"

MeanShift::MeanShift(float bandwidth, type_kernel kernel, short int max_iterations) {
  this->bandwidth = bandwidth * bandwidth;
  this->max_iterations = max_iterations;
  if (kernel == gaussian)
    this->kernel = new KernelGaussian();
  else
    this->kernel = new KernelFlat();
}

MeanShift::~MeanShift() {}

float MeanShift::get_bandwidth() { return sqrt(bandwidth); }
std::vector<unsigned short int> MeanShift::get_labels() { return labels; }
type_kernel MeanShift::get_type_kernel() {
  if (typeid(*kernel) == typeid(KernelGaussian))
    return gaussian;
  else
    return flat;
}