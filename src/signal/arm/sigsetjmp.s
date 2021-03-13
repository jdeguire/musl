.syntax unified
.global sigsetjmp
.global __sigsetjmp
.type sigsetjmp,%function
.type __sigsetjmp,%function
sigsetjmp:
__sigsetjmp:
	tst r1,r1
	bne 1f
	b setjmp

1:	mov r2,lr
	adds r0,#200
	str r2,[r0,#56]
	str r4,[r0,#60+8]
	mov r4,r0

	bl setjmp

	mov r1,r0
	mov r0,r4
	ldr r2,[r0,#56]
	mov lr,r2
	ldr r4,[r0,#60+8]
	subs r0,#200

.hidden __sigsetjmp_tail
	b __sigsetjmp_tail
