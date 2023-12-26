.text
.code64
1:
    mov $0x3f8, %dx
    mov $'B', %ax
    out %ax, %dx
    jmp 1b
