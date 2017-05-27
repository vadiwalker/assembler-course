global memcpy16
;1-rdi
;2-rsi
;3-rdx
memcpy16:
	push rcx

loop:
	cmp rdx, 16
	jl fill
	movdqa xmm0, [rsi]
	movdqa [rdi], xmm0
	
	add rsi, 16
	add rdi, 16
	sub rdx, 16

	jmp loop

fill:
	cmp rdx, 0
	jng finish
	mov cl, [rsi]
	mov [rdi], cl

	add rsi, 1
	add rdi, 1
	sub rdx, 1

	jmp fill

finish: 
	pop rcx
	sfence
	ret