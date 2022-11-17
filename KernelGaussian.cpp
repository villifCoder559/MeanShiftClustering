#include "KernelGaussian.h"

KernelGaussian::KernelGaussian(){}
KernelGaussian::~KernelGaussian(){}
float KernelGaussian::compute(float distance, float bandwith) {
  return exp((-0.5 * pow(distance, 2) / (pow(bandwith, 2))));
}
