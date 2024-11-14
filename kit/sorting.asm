MOV SI, 0500
mov cl , [si]
dec cl
:040f mov si 0500
mov ch [si]
dec ch
inc si
040f:  mov al,[si]
inc si
cmp al,[si]
jc 041c
xchg al,[si]
dec si
xchg al,[si]
inc si
041c: dec ch
jnz 040f
dec cl
jnz 0407
hlt
