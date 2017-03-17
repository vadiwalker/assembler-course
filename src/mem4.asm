global memcpy4
;rdi - 1 
;rsi - 2
;rdx - 3

memcpy4:
loop:
	cmp rdx, 0
	jng finish
	add rdx, 4

	

	jmp loop
finish:
	ret	