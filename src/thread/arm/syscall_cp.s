.syntax unified
.global __cp_begin
.hidden __cp_begin
.global __cp_end
.hidden __cp_end
.global __cp_cancel
.hidden __cp_cancel
.hidden __cancel
.global __syscall_cp_asm
.hidden __syscall_cp_asm
.type __syscall_cp_asm,%function
__syscall_cp_asm:
	mov ip,sp
	push {r4,r5,r6,r7}
__cp_begin:
	ldr r0,[r0]
	cmp r0,#0
	bne __cp_cancel
	mov r7,r1
	mov r0,r2
	mov r1,r3
	mov r2,ip
	ldmfd r2,{r2,r3,r4,r5,r6}
	svc 0
__cp_end:
	pop {r4,r5,r6,r7}
	bx lr
__cp_cancel:
	pop {r4,r5,r6,r7}
	b __cancel
