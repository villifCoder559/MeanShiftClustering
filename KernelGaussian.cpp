#include "KernelGaussian.h"

KernelGaussian::KernelGaussian() {}
KernelGaussian::~KernelGaussian() {}
float KernelGaussian::compute(float distance, float bandwith) {
  float value = exp(((distance*distance) / (bandwith*bandwith)));
  return value > 0.000001 ? value : 0;
}
