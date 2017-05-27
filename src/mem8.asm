global memcpy8

;1-rdi
;2-rsi
;3-rdx
;rsi -> rdi

memcpy8:
	push rcx

loop:
	cmp rdx, 8	
	jl fill
	mov rcx, [rsi]
	mov [rdi], rcx
	add rsi, 8
	add rdi, 8
	sub rdx, 8

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