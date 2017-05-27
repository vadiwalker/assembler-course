global count_words
;rdi - 1
;rsi - 1
count_words:
	
	mov rax, 0
	mov eax, 4
	mov ebx, 2
	div ebx
	mov eax, ebx
	
finish:	
	ret