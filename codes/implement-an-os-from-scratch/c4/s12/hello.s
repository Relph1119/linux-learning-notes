.text
.code16
start:
    mov $stack, %sp
    # 发起函数调用
    call sum

    mov $0x3f8, %dx
    mov $0x41, %al
    out %al, %dx
    hlt

.org 0x1000
stack:
