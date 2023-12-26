# 立即数寻址
.text
.code16
start:
    mov $0x41, %al
    jmp 1f
    mov $0x42, %al
1:
    # 将0x41写到串口
    mov $0x3f8, %dx
    out %al, %dx
    hlt    