#include <math.h>

#if __ARM_PCS_VFP && !BROKEN_VFP_ASM && (__ARM_FP&4)

float fabsf(float x)
{
	__asm__ ("vabs.f32 %0, %1" : "=t"(x) : "t"(x));
	return x;
}

#else

#include "../fabsf.c"

#endif
