.section	__TEXT,__text,regular,pure_instructions

## 0 -> a
.global __al_bi_clear
.align	4, 0x90
__al_bi_clear:
	.cfi_startproc
		## size_a: rdi
		## buf_a : rsi
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	movq	%rdi, %rcx
	movq 	%rsi, %rdi
	subb 	%al, %al
loop1_1:
	movb 	%al, (%rdi)
	incq	%rdi
	decq	%rcx
	jnz 	loop1_1
	popq	%rbp
	retq
	.cfi_endproc

## size_a == size_b
## a -> b
.global __al_bi_copy
.align	4, 0x90
__al_bi_copy:
	.cfi_startproc
	pushq	%rbp
Ltmp3:
	.cfi_def_cfa_offset 16
Ltmp4:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp5:
	.cfi_def_cfa_register %rbp
		## size_a: rdi
		## buf_a : rsi
		## buf_b : rdx
	movq	%rdi, %rcx
	movq	%rdx, %rdi
loop2_1:
	movb	(%rsi), %al
	movb	%al, (%rdi)
	incq	%rsi
	incq	%rdi
	decq	%rcx
	jnz 	loop2_1
	popq	%rbp
	retq
	.cfi_endproc

## size_a == size_b
## a << 1 -> b
.global __al_bi_shiftL
.align	4, 0x90
__al_bi_shiftL:
	.cfi_startproc
		## size_a: rdi
		## buf_a : rsi
		## buf_b : rdx
	pushq	%rbp
Ltmp6:
	.cfi_def_cfa_offset 16
Ltmp7:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp8:
	.cfi_def_cfa_register %rbp
	movq	%rdi, %rcx
	movq	%rdx, %rdi
	addq	%rcx, %rsi
	addq	%rcx, %rdi
	subb	%al, %al
loop3_1:
	decq	%rsi
	decq	%rdi
	movb	(%rsi), %al
	rclb	$1, %al
	movb	%al, (%rdi)
	decq	%rcx
	jnz 	loop3_1
	popq	%rbp
	retq
	.cfi_endproc

## size_a == size_b
## a >> 1 -> b
.global __al_bi_shiftR
.align	4, 0x90
__al_bi_shiftR:
	.cfi_startproc
		## size_a: rdi
		## buf_a : rsi
		## buf_b : rdx
	pushq	%rbp
Ltmp9:
	.cfi_def_cfa_offset 16
Ltmp10:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp11:
	.cfi_def_cfa_register %rbp
	movq	%rdi, %rcx
	movq	%rdx, %rdi
	subb	%al, %al
loop4_1:
	movb	(%rsi), %al
	rcrb	$1, %al
	movb	%al, (%rdi)
	incq	%rsi
	incq	%rdi
	decq	%rcx
	jnz 	loop4_1
	popq	%rbp
	retq
	.cfi_endproc

## size_a >= size_b
## a + b -> a
## if overflow, return 1
.global __al_bi_add
.align	4, 0x90
__al_bi_add:
	.cfi_startproc
		## size_a: rdi
		## buf_a : rsi
		## size_b: rdx
		## buf_b : rcx
	pushq	%rbp
Ltmp12:
	.cfi_def_cfa_offset 16
Ltmp13:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp14:
	.cfi_def_cfa_register %rbp
	xchgq	%rdi, %rcx
	xchgq	%rdi, %rsi
	addq	%rcx, %rdi
	addq	%rdx, %rsi
	subq	%rdx, %rcx
	incq	%rcx
	subb	%al, %al
	movq	$0, %r10
loop5_1:
	decq	%rdi
	decq	%rsi
	movb	(%rdi), %al
	adcb	(%rsi), %al
	movb	%al, (%rdi)
	decq	%rdx
	jnz 	loop5_1
	jnc 	skip5_2
loop5_2:
	decq	%rcx
	jz		skip5_1
	decq	%rdi
	movb	(%rdi), %al
	adcb	$0, %al
	movb	%al, (%rdi)
	jnc 	skip5_2
	jmp 	loop5_2
skip5_1:
	movq	$1, %r10
skip5_2:
	movq	%r10,%rax
	popq	%rbp
	retq
	.cfi_endproc

## size_a >= size_b
## a - b -> a
## if negative, return 1
.global __al_bi_sub
.align	4, 0x90
__al_bi_sub:
	.cfi_startproc
		## size_a: rdi
		## buf_a : rsi
		## size_b: rdx
		## buf_b : rcx
	pushq	%rbp
Ltmp15:
	.cfi_def_cfa_offset 16
Ltmp16:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp17:
	.cfi_def_cfa_register %rbp
	xchgq	%rdi, %rcx
	xchgq	%rdi, %rsi
	addq	%rcx, %rdi
	addq	%rdx, %rsi
	subq	%rdx, %rcx
	incq	%rcx
	subb	%al ,%al
	movq	$0, %r10
loop6_1:
	decq	%rdi
	decq	%rsi
	movb	(%rdi), %al
	sbbb	(%rsi), %al
	movb	%al, (%rdi)
	decq	%rdx
	jnz 	loop6_1
	jnc 	skip6_2
loop6_2:
	decq	%rcx
	jz		skip6_1
	decq	%rdi
	movb	(%rdi), %al
	sbbb	$0, %al
	movb	%al, (%rdi)
	jnc 	skip6_2
	jmp 	loop6_2
skip6_1:
	movq	$1, %r10
skip6_2:
	movq	%r10, %rax
	popq	%rbp
	retq
	.cfi_endproc

## size_a == size_b
## 0 - a -> b
.global __al_bi_neg
.align	4, 0x90
__al_bi_neg:
	.cfi_startproc
		## size_a: rdi
		## buf_a : rsi
		## buf_b : rdx
	pushq	%rbp
Ltmp18:
	.cfi_def_cfa_offset 16
Ltmp19:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp20:
	.cfi_def_cfa_register %rbp
	movq	%rdi, %rcx
	movq	%rdx, %rdi
	addq	%rcx, %rsi
	addq	%rcx, %rdi
	subb	%al, %al
loop7_1:
	decq	%rsi
	decq	%rdi
	movb	$0, %al
	sbbb	(%rsi), %al
	movb	%al, (%rdi)
	decq	%rcx
	jnz 	loop7_1
	popq	%rbp
	retq
	.cfi_endproc

.global __al_bi_cut
.align	4, 0x90
__al_bi_cut:
	.cfi_startproc
		## size_a: rdi
		## buf_a : rsi
	pushq	%rbp
Ltmp21:
	.cfi_def_cfa_offset 16
Ltmp22:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp23:
	.cfi_def_cfa_register %rbp
	movq	%rdi, %rcx
loop8_1:
	movb	(%rsi), %al
	orb		%al, %al
	jnz 	skip8_1
	incq 	%rsi
	decq 	%rcx
	jnz	loop8_1
skip8_1:
	movq	%rcx, %rax
	popq	%rbp
	retq
	.cfi_endproc

## if a > b, return 1
## if a == b, return 0
## if a < b, return -1
.global __al_bi_comp
.align	4, 0x90
__al_bi_comp:
	.cfi_startproc
		## size_a: rdi
		## buf_a : rsi
		## size_b: rdx
		## buf_b : rcx
	pushq	%rbp
Ltmp24:
	.cfi_def_cfa_offset 16
Ltmp25:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp26:
	.cfi_def_cfa_register %rbp
	xchg	%rdi, %rcx
	xchg	%rdi, %rsi
	cmpq	%rcx, %rdx
	jnc		skip9_1
	movq	$1, %rax
	popq	%rbp
	retq
skip9_1:
	cmpq	%rdx, %rcx
	jnc		skip9_2
	movq	$-1, %rax
	popq	%rbp
	retq
skip9_2:
loop9_1:
	movb	(%rdi), %al
	cmpb	(%rsi), %al
	jc		skip9_3
	jnz 	skip9_4
	incq	%rdi
	incq	%rsi
	decq	%rcx
	jnz 	loop9_1
	movq	$0, %r10
	jmp 	skip9_5
skip9_3:
	movq	$-1, %r10
	jmp 	skip9_5
skip9_4:
	movq	$1, %r10
skip9_5:
	movq	%r10, %rax
	popq	%rbp
	retq
	.cfi_endproc

.subsections_via_symbols
