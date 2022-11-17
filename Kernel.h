#ifndef _KERNEL_
#define _KERNEL_


class Kernel {
 public:
  virtual ~Kernel() = 0;
  virtual float compute(float distance, float bandwith)=0;
};
#endif