.code

; 0 -> a 
_al_bi_clear PROC
		; size_a: rcx
		; buf_a : rdx
	push	rdi
	mov 	rdi, rdx
	sub 	al, al
loop1:
	mov 	byte ptr [rdi], al
	inc 	rdi
	dec 	rcx
	jnz 	loop1
	pop 	rdi
	ret
_al_bi_clear ENDP

; size_a == size_b
; a -> b
_al_bi_copy PROC
		; size_a: rcx
		; buf_a : rdx
		; buf_b : r8
	push	rsi
	push	rdi
	mov 	rsi, rdx
	mov 	rdi, r8
loop1:
	mov 	al, byte ptr [rsi]
	mov 	byte ptr [rdi], al
	inc 	rsi
	inc 	rdi
	dec 	rcx
	jnz 	loop1
	pop 	rdi
	pop 	rsi
	ret
_al_bi_copy ENDP


; size_a == size_b
; a << 1 -> b
_al_bi_shiftL PROC
		; size_a: rcx
		; buf_a : rdx
		; buf_b : r8
	push	rsi
	push	rdi
	mov 	rsi, rdx
	add 	rsi, rcx
	mov 	rdi, r8
	add 	rdi, rcx
	sub 	al, al
loop1:
	dec 	rsi
	dec 	rdi
	mov 	al, byte ptr [rsi]
	rcl 	al, 1
	mov 	byte ptr [rdi], al
	dec 	rcx
	jnz 	loop1
	pop 	rdi
	pop 	rsi
	ret
_al_bi_shiftL ENDP

; size_a == size_b
; a >> 1 -> b
_al_bi_shiftR PROC
		; size_a: rcx
		; buf_a : rdx
		; buf_b : r8
	push	rsi
	push	rdi
	mov 	rsi, rdx
	mov 	rdi, r8
	sub 	al, al
loop1:
	mov 	al, byte ptr [rsi]
	rcr 	al, 1
	mov 	byte ptr [rdi], al
	inc 	rsi
	inc 	rdi
	dec 	rcx
	jnz 	loop1
	pop 	rdi
	pop 	rsi
	ret
_al_bi_shiftR ENDP

; size_a >= size_b
; a + b -> a
; if overflow, return 1
_al_bi_add PROC
		; size_a: rcx
		; buf_a : rdx
		; size_b: r8
		; buf_b : r9
	push	rsi
	push	rdi
	mov 	rdi, rdx
	add 	rdi, rcx
	mov 	rdx, r8
	mov 	rsi, r9
	add 	rsi, rdx
	sub 	rcx, rdx
	inc 	rcx
	sub 	al, al
	mov		r10, 0
loop1:
	dec 	rdi
	dec 	rsi
	mov 	al, byte ptr [rdi]
	adc 	al, byte ptr [rsi]
	mov 	byte ptr [rdi], al
	dec 	rdx
	jnz 	loop1
	jnc 	skip2
loop2:
	dec 	rcx
	jz		skip1
	dec 	rdi
	mov 	al, byte ptr [rdi]
	adc 	al, 0
	mov 	byte ptr [rdi], al
	jnc 	skip2
	jmp 	loop2
skip1:
	mov 	r10, 1
skip2:
	mov		rax, r10
	pop 	rdi
	pop 	rsi
	ret
_al_bi_add ENDP

; size_a >= size_b
; a - b -> a
; if negative, return 1
_al_bi_sub PROC
		; size_a: rcx
		; buf_a : rdx
		; size_b: r8
		; buf_b : r9
	push	rsi
	push	rdi
	mov 	rdi, rdx
	add 	rdi, rcx
	mov 	rdx, r8
	mov 	rsi, r9
	add 	rsi, rdx
	sub 	rcx, rdx
	inc 	rcx
	sub 	al ,al
	mov		r10, 0
loop1:
	dec 	rdi
	dec 	rsi
	mov 	al, byte ptr [rdi]
	sbb 	al, byte ptr [rsi]
	mov 	byte ptr [rdi], al
	dec 	rdx
	jnz 	loop1
	jnc 	skip2
loop2:
	dec 	rcx
	jz		skip1
	dec 	rdi
	mov 	al, byte ptr [rdi]
	sbb 	al, 0
	mov 	byte ptr [rdi], al
	jnc 	skip2
	jmp 	loop2
skip1:
	mov 	r10, 1
skip2:
	mov		rax, r10
	pop 	rdi
	pop 	rsi
	ret
_al_bi_sub ENDP

; size_a == size_b
; 0 - a -> b
_al_bi_neg PROC
		; size_a: rcx
		; buf_a : rdx
		; buf_b : r8
	push	rsi
	push	rdi
	mov 	rsi, rdx
	add 	rsi, rcx
	mov 	rdi, r8
	add 	rdi, rcx
	sub 	al, al
loop1:
	dec 	rsi
	dec 	rdi
	mov 	al, 0
	sbb 	al, byte ptr [rsi]
	mov 	byte ptr [rdi], al
	dec 	rcx
	jnz 	loop1
	pop 	rdi
	pop 	rsi
	ret
_al_bi_neg ENDP

_al_bi_cut PROC
		; size_a: rcx
		; buf_a : rdx
	push	rsi
	mov 	rsi, rdx
loop1:
	mov 	al, byte ptr [rsi]
	or		al, al
	jnz 	skip1
	inc 	rsi
	dec 	rcx
	jnz 	loop1
skip1:
	mov 	rax, rcx
	pop 	rsi
	ret
_al_bi_cut ENDP

; if a > b, return 1
; if a == b, return 0
; if a < b, return -1
_al_bi_comp PROC
		; size_a: rcx
		; buf_a : rdx
		; size_b: r8
		; buf_b : r9
	cmp		r8, rcx
	jnc		skip1
	mov		rax, 1
	ret
skip1:
	cmp		rcx, r8
	jnc		skip2
	mov		rax, -1
	ret
skip2:
	push	rsi
	push	rdi
	mov 	rdi, rdx
	mov 	rsi, r9
loop1:
	mov 	al, byte ptr [rdi]
	cmp 	al, byte ptr [rsi]
	jc		skip3
	jnz 	skip4
	inc 	rdi
	inc 	rsi
	dec 	rcx
	jnz 	loop1
	mov		r10, 0
	jmp 	skip5
skip3:
	mov 	r10, -1
	jmp 	skip5
skip4:
	mov 	r10, 1
skip5:
	pop 	rdi
	pop 	rsi
	mov		rax, r10
	ret
_al_bi_comp ENDP

end
