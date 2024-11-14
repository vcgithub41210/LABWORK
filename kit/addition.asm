mov si,2000
mov di,3000
mov ax,[si]
inc si
inc si
mov bx,[si]
add bx
jc (mov [di], 0001)
mov [di],0000
jmp (inc di)
mov [di],0001
inc di
mov [di],ax
hlt
