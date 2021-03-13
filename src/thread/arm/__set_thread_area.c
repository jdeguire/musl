#include <stdint.h>
#include <elf.h>
#include "pthread_impl.h"
#include "libc.h"

#define HWCAP_TLS (1 << 15)

extern hidden const unsigned char
	__a_barrier_oldkuser[], __a_barrier_v6[], __a_barrier_v7[], __a_barrier_m[],
	__a_cas_v6[], __a_cas_v7[], __a_cas_m[], __a_cas_intmask_m[],
	__a_gettp_cp15[], __a_gettp_cp15_m[];

#define __a_barrier_kuser 0xffff0fa0
#define __a_barrier_oldkuser (uintptr_t)__a_barrier_oldkuser
#define __a_barrier_v6 (uintptr_t)__a_barrier_v6
#define __a_barrier_v7 (uintptr_t)__a_barrier_v7
#define __a_barrier_m (uintptr_t)__a_barrier_m

#define __a_cas_kuser 0xffff0fc0
#define __a_cas_v6 (uintptr_t)__a_cas_v6
#define __a_cas_v7 (uintptr_t)__a_cas_v7
#define __a_cas_m (uintptr_t)__a_cas_m
#define __a_cas_intmask_m (uintptr_t)__a_cas_intmask_m

#define __a_gettp_kuser 0xffff0fe0
#define __a_gettp_cp15 (uintptr_t)__a_gettp_cp15
#define __a_gettp_cp15_m (uintptr_t)__a_gettp_cp15_m

extern hidden uintptr_t __a_barrier_ptr, __a_cas_ptr, __a_gettp_ptr;

static long __a_gettp_syscall(void)
{
	return __syscall(__ARM_NR_get_tls);
}

int __set_thread_area(void *p)
{
#if __ARM_ARCH_PROFILE == 'M'
	__a_cas_ptr = __a_cas_m;
	__a_barrier_ptr = __a_barrier_m;

	if (__hwcap & HWCAP_TLS) {
		__a_gettp_ptr = __a_gettp_cp15_m;
	} else {
		size_t *aux;
		__a_gettp_ptr = __a_gettp_syscall;
		for (aux=libc.auxv; *aux; aux+=2) {
			if (*aux != AT_PLATFORM) continue;
			const char *s = (void *)aux[1];
			if (s[0]=='v' && s[1]=='6') {
				__a_cas_ptr = __a_cas_intmask_m;
				break;
			}
		}
	}
#elif !__ARM_ARCH_7A__ && !__ARM_ARCH_7R__ && __ARM_ARCH < 7
	if (__hwcap & HWCAP_TLS) {
		size_t *aux;
		__a_cas_ptr = __a_cas_v7;
		__a_barrier_ptr = __a_barrier_v7;
		for (aux=libc.auxv; *aux; aux+=2) {
			if (*aux != AT_PLATFORM) continue;
			const char *s = (void *)aux[1];
			if (s[0]!='v' || s[1]!='6' || s[2]-'0'<10u) break;
			__a_cas_ptr = __a_cas_v6;
			__a_barrier_ptr = __a_barrier_v6;
			break;
		}
	} else {
		int ver = *(int *)0xffff0ffc;
		__a_gettp_ptr = __a_gettp_kuser;
		__a_cas_ptr = __a_cas_kuser;
		__a_barrier_ptr = __a_barrier_kuser;
		if (ver < 2) a_crash();
		if (ver < 3) __a_barrier_ptr = __a_barrier_oldkuser;
	}
#endif
	return __syscall(0xf0005, p);
}
