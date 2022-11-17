#ifndef _K_FLAT_
#define _K_FLAT_
#include "Kernel.h"
#include <math.h>
class KernelFlat: public Kernel{
    public:
    KernelFlat();
	float compute(float distance,float bandwith);
    ~KernelFlat();
};
#endif