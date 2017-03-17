global memcpy32

memcpy32:
loop:
	cmp rdx, 0
	jng finish
	sub rdx, 32

	
	add rsi, 32
	add rdi, 32

	jmp loop
finish:
	ret	