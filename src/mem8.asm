global memcpy8
;1-rdi
;2-rsi
;3-rdx
memcpy8:
	push rcx
loop:
	cmp rdx, 0	
	jng finish
	sub rdx, 8
	mov rcx, [rdi]
	mov [rsi], rcx
	add rsi, 8
	add rdi, 8

	jmp loop
finish: 
	pop rcx
	ret