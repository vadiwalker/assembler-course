global memcpy4
;rdi - 1 
;rsi - 2
;rdx - 3

memcpy4:
	push rcx


loop:
	cmp rdx, 4
	jl fill
	mov ecx, [rsi]
	mov [rdi], ecx
	add rsi, 4
	add rdi, 4
	sub rdx, 4
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
	ret	