global memcpy16
;1-rdi
;2-rsi
;3-rdx
memcpy16:

loop:
	cmp rdx, 0	
	jng finish
	sub rdx, 16
	movdqa xmm0, [rdi]
	movdqa [rsi], xmm0
	add rsi, 16
	add rdi, 16
	jmp loop
finish: 
	sfence
	ret