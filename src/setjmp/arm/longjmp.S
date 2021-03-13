.syntax unified
.global _longjmp
.global longjmp
.type _longjmp,%function
.type longjmp,%function
_longjmp:
longjmp:
	mov ip,r0
	movs r0,r1
#if defined(__thumb2__)  ||  !defined(__thumb__)
	moveq r0,#1
	ldmia ip!, {v1,v2,v3,v4,v5,v6,sl,fp}
	ldmia ip!, {r2,lr}
	mov sp,r2
#else
	bne 4f
	movs r0,#1
4:	mov r1,ip
	adds r1,#16
	ldmia r1!, {r2-r7}
	mov lr,r7
	mov sp,r6
	mov r11,r5
	mov r10,r4
	mov r9,r3
	mov r8,r2
	mov ip,r1
	subs r1,#40
	ldmia r1!, {r4-r7}
#endif
	adr r1,1f
	ldr r2,1f
	ldr r1,[r1,r2]

#if __ARM_ARCH_PROFILE != 'M' && __ARM_ARCH < 8
	tst r1,#0x260
	beq 3f
	// HWCAP_ARM_FPA
	tst r1,#0x20
	beq 2f
	ldc p2, cr4, [ip], #48
#endif
2:	movs r2,#0x40
	tst r1,r2
	beq 2f
	.fpu vfp
	vldmia ip!, {d8-d15}
	.fpu softvfp
	.eabi_attribute 10, 0
	.eabi_attribute 27, 0
#if __ARM_ARCH_PROFILE != 'M' && __ARM_ARCH < 8
	// HWCAP_ARM_IWMMXT
2:	tst r1,#0x200
	beq 3f
	ldcl p1, cr10, [ip], #8
	ldcl p1, cr11, [ip], #8
	ldcl p1, cr12, [ip], #8
	ldcl p1, cr13, [ip], #8
	ldcl p1, cr14, [ip], #8
	ldcl p1, cr15, [ip], #8
#endif
2:
3:	bx lr

.hidden __hwcap
.align 2
1:	.word __hwcap-1b
