global memcpy1

memcpy1:
	push rcx
loop:
	cmp rdx, 0
	jng finish 
	mov cl, [rsi]
	mov [rdi], cl
	add rsi, 1
	add rdi, 1
	sub rdx, 1
	jmp loop
finish:
	pop rcx
	ret