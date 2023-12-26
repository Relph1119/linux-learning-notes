.text
.code64
# 无限循环，每次循环向串口输出一个字符
1:
    mov $0x3f8, %dx
    mov $'A', %ax
    out %ax, %dx
    jmp 1b