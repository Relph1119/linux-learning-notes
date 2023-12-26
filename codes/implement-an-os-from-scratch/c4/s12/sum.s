.text
.code16
# 全局可见
.global sum
sum:
    push %bp
    mov %sp, %bp

    mov $1, %bx
    mov $0, %cx
    sub $8, %sp
1:
    add %bx, %cx
    inc %bx
    cmp $10, %bx
    jbe 1b

    mov $0x3f8, %dx
    mov %cx, %ax
    out %ax, %dx

    mov %bp, %sp
    pop %bp
    ret  