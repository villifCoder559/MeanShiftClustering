#ifndef _K_GAUSS_
#define _K_GAUSS_
#include "Kernel.h"
#include <math.h>
class KernelGaussian: public Kernel{
	public:
	KernelGaussian();
	float compute(float distance,float bandwith);
	~KernelGaussian();
};
#endif