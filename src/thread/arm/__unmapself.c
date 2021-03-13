#if __ARM_ARCH_PROFILE != 'M'

#include "pthread_impl.h"

void __unmapself(void *base, size_t size)
{
	register void *r0 __asm__("r0") = base;
	register size_t r1 __asm__("r1") = size;
	__asm__ __volatile__ (
	"	movs r7,#91 \n"
	"	svc 0 \n"
	"	movs r7,#1 \n"
	"	svc 0 \n"
	:: "r"(r0), "r"(r1));
}

#else

#include "../__unmapself.c"

#endif
