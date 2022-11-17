#include "KernelFlat.h"

KernelFlat::KernelFlat(){}
KernelFlat::~KernelFlat(){}
float KernelFlat::compute(float distance, float bandwith) {
  return (distance < bandwith ? 1 : 0);
}