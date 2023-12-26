.text
.code64
# 无限循环，每次循环向串口输出一个字符
1:  
    movb $'F', (0x12345678)
    mov (0x12345678), %al

    mov $0x3f8, %dx
    out %al, %dx

    jmp 1b