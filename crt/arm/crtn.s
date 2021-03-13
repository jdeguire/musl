.syntax unified

.section .init
	pop {r0,r1}
	bx r1

.section .fini
	pop {r0,r1}
	bx r1
