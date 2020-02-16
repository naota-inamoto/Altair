section .text

; 0 -> a 
global _al_bi_clear
_al_bi_clear:
		; size_a: rdi
		; buf_a : rsi
	mov		rcx, rdi
	mov 	rdi, rsi
	sub 	al, al
loop1_1:
	mov 	byte [rdi], al
	inc 	rdi
	dec 	rcx
	jnz 	loop1_1
	ret

; size_a == size_b
; a -> b
global _al_bi_copy
_al_bi_copy:
		; size_a: rdi
		; buf_a : rsi
		; buf_b : rdx
	mov 	rcx, rdi
	mov 	rdi, rdx
loop2_1:
	mov 	al, byte [rsi]
	mov 	byte [rdi], al
	inc 	rsi
	inc 	rdi
	dec 	rcx
	jnz 	loop2_1
	ret

; size_a == size_b
; a << 1 -> b
global _al_bi_shiftL
_al_bi_shiftL:
		; size_a: rdi
		; buf_a : rsi
		; buf_b : rdx
	mov		rcx, rdi
	mov 	rdi, rdx
	add 	rsi, rcx
	add 	rdi, rcx
	sub 	al, al
loop3_1:
	dec 	rsi
	dec 	rdi
	mov 	al, byte [rsi]
	rcl 	al, 1
	mov 	byte [rdi], al
	dec 	rcx
	jnz 	loop3_1
	ret

; size_a == size_b
; a >> 1 -> b
global _al_bi_shiftR
_al_bi_shiftR:
		; size_a: rdi
		; buf_a : rsi
		; buf_b : rdx
	mov		rcx, rdi
	mov 	rdi, rdx
	sub 	al, al
loop4_1:
	mov 	al, byte [rsi]
	rcr 	al, 1
	mov 	byte [rdi], al
	inc 	rsi
	inc 	rdi
	dec 	rcx
	jnz 	loop4_1
	ret

; size_a >= size_b
; a + b -> a
; if overflow, return 1
global _al_bi_add
_al_bi_add:
		; size_a: rdi
		; buf_a : rsi
		; size_b: rdx
		; buf_b : rcx
	xchg	rcx, rdi
	xchg	rsi, rdi
	add 	rdi, rcx
	add 	rsi, rdx
	sub 	rcx, rdx
	inc 	rcx
	sub 	al, al
	mov		r10, 0
loop5_1:
	dec 	rdi
	dec 	rsi
	mov 	al, byte [rdi]
	adc 	al, byte [rsi]
	mov 	byte [rdi], al
	dec 	rdx
	jnz 	loop5_1
	jnc 	skip5_2
loop5_2:
	dec 	rcx
	jz		skip5_1
	dec 	rdi
	mov 	al, byte [rdi]
	adc 	al, 0
	mov 	byte [rdi], al
	jnc 	skip5_2
	jmp 	loop5_2
skip5_1:
	mov 	r10, 1
skip5_2:
	mov		rax, r10
	ret

; size_a >= size_b
; a - b -> a
; if negative, return 1
global _al_bi_sub
_al_bi_sub:
		; size_a: rdi
		; buf_a : rsi
		; size_b: rdx
		; buf_b : rcx
	xchg	rcx, rdi
	xchg	rsi, rdi
	add 	rdi, rcx
	add 	rsi, rdx
	sub 	rcx, rdx
	inc 	rcx
	sub 	al ,al
	mov		r10, 0
loop6_1:
	dec 	rdi
	dec 	rsi
	mov 	al, byte [rdi]
	sbb 	al, byte [rsi]
	mov 	byte [rdi], al
	dec 	rdx
	jnz 	loop6_1
	jnc 	skip6_2
loop6_2:
	dec 	rcx
	jz		skip6_1
	dec 	rdi
	mov 	al, byte [rdi]
	sbb 	al, 0
	mov 	byte [rdi], al
	jnc 	skip6_2
	jmp 	loop6_2
skip6_1:
	mov 	r10, 1
skip6_2:
	mov		rax, r10
	ret

; size_a == size_b
; 0 - a -> b
global _al_bi_neg
_al_bi_neg:
		; size_a: rdi
		; buf_a : rsi
		; buf_b : rdx
	mov		rcx, rdi
	mov 	rdi, rdx
	add 	rsi, rcx
	add 	rdi, rcx
	sub 	al, al
loop7_1:
	dec 	rsi
	dec 	rdi
	mov 	al, 0
	sbb 	al, byte [rsi]
	mov 	byte [rdi], al
	dec 	rcx
	jnz 	loop7_1
	ret

global _al_bi_cut
_al_bi_cut:
		; size_a: rdi
		; buf_a : rsi
	mov 	rcx, rdi
loop8_1:
	mov 	al, byte [rsi]
	or		al, al
	jnz 	skip8_1
	inc 	rsi
	dec 	rcx
	jnz 	loop8_1
skip8_1:
	mov 	rax, rcx
	ret

; if a > b, return 1
; if a == b, return 0
; if a < b, return -1
global _al_bi_comp
_al_bi_comp:
		; size_a: rdi
		; buf_a : rsi
		; size_b: rdx
		; buf_b : rcx
	xchg	rcx, rdi
	xchg	rsi, rdi
	cmp		rdx, rcx
	jnc		skip9_1
	mov		rax, 1
	ret
skip9_1:
	cmp		rcx, rdx
	jnc		skip9_2
	mov		rax, -1
	ret
skip9_2:
loop9_1:
	mov 	al, byte [rdi]
	cmp 	al, byte [rsi]
	jc		skip9_3
	jnz 	skip9_4
	inc 	rdi
	inc 	rsi
	dec 	rcx
	jnz 	loop9_1
	mov		r10, 0
	jmp 	skip9_5
skip9_3:
	mov 	r10, -1
	jmp 	skip9_5
skip9_4:
	mov 	r10, 1
skip9_5:
	mov		rax, r10
	ret

