.syntax unified

.global __restore
.hidden __restore
.type __restore,%function
__restore:
	movs r7,#119
	swi 0x0

.global __restore_rt
.hidden __restore_rt
.type __restore_rt,%function
__restore_rt:
	movs r7,#173
	swi 0x0
