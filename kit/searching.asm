mov si,1200
mov cl,[si]
inc si
mov bl,[1300]
mov al,[si]
cmp al,bl
jz (mov [1400],01)
inc si
dec cl
jz (hlt)
jnz (mov al,[si])
mov [1400],01
hlt
