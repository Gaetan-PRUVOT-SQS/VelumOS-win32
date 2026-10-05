.intel_syntax noprefix
.altmacro

.set MSR_GS_BASE, 0xc0000101
.set FRAME_CS, 24
.set REGS_SIZE, 176

.macro SAVE_REGS
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
.endm

.macro RESTORE_REGS
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
.endm

.macro ISR_STUB vec
	.balign 16
1:
	.if (\vec != 8) && (\vec != 10) && (\vec != 11) && (\vec != 12)
	.if (\vec != 13) && (\vec != 14) && (\vec != 17) && (\vec != 21)
	.if (\vec != 29) && (\vec != 30)
	push 0
	.endif
	.endif
	.endif
	push \vec
	.if (\vec == 2) || (\vec == 8) || (\vec == 18)
	jmp isr_paranoid
	.else
	jmp isr_common
	.endif
	.pushsection .rodata
	.quad 1b
	.popsection
.endm

.section .rodata
.balign 8
isr_stubs:

.text
.balign 16
isr_common:
	test byte ptr [rsp + FRAME_CS], 3
	jz 1f
	swapgs
1:
	lfence
	SAVE_REGS
	cld
	mov rdi, rsp
	call trap_dispatch
	RESTORE_REGS
	test byte ptr [rsp + FRAME_CS], 3
	jz 2f
	swapgs
2:
	add rsp, 16
	iretq

.balign 16
isr_paranoid:
	SAVE_REGS
	cld
	mov ecx, MSR_GS_BASE
	rdmsr
	shl rdx, 32
	or rax, rdx
	mov r12, rax
	mov rax, [rsp + REGS_SIZE]
	mov rdx, rax
	shr rdx, 32
	wrmsr
	lfence
	mov rdi, rsp
	call trap_dispatch
	mov ecx, MSR_GS_BASE
	mov rax, r12
	mov rdx, r12
	shr rdx, 32
	wrmsr
	RESTORE_REGS
	add rsp, 16
	iretq

.set vec, 0
.rept 256
	ISR_STUB %vec
	.set vec, vec + 1
.endr

.globl isr_stub_table
.type isr_stub_table, @function
isr_stub_table:
	lea rax, [rip + isr_stubs]
	ret
.size isr_stub_table, . - isr_stub_table

.section .note.GNU-stack,"",@progbits
