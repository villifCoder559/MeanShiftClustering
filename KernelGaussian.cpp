#include "KernelGaussian.h"

KernelGaussian::KernelGaussian() {}
KernelGaussian::~KernelGaussian() {}
float KernelGaussian::compute(float distance, float bandwith) {
  float value = exp((-0.5 * pow(distance, 2) / (pow(bandwith, 2))));
  return value > 0.00001 ? value : 0;
}
