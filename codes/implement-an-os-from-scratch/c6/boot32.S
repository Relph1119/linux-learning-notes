.text
.code32
start32:
    # 向串口输出一个字符P
    mov $0x3f8, %dx
    mov $'P', %al
    out %al, %dx

    hlt
