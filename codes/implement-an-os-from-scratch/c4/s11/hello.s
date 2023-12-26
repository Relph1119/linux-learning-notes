# 栈
.text
.code16
start:
    mov $stack, %sp
    mov $0x3f8, %dx

    # 向栈中压入0x41和0x42
    push $0x41
    push $0x42

    # 打印输出栈的值
    mov 0x1000 - 2, %ax
    out %ax, %dx
    mov 0x1000 - 4, %ax
    out %ax, %dx

    # 弹出栈
    pop %ax
    out %ax, %dx
    pop %ax
    out %ax, %dx

    hlt

.ort 0x1000
stack: