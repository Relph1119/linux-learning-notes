# SIB + disp寻址
.text
.code16
start:
    # 使用EBX寄存器存储行基址
    mov $0, %ebx
    # 使用ESI存储数组的索引
    mov $1, %esi
    mov $0, %ax

1:
    # 将数组元素累加到AX寄存器中
    add var(%ebx, %esi, 2), %ax

    # 指向数组的下一行基址
    add $6, %ebx
    cmp $12, %ebx
    jbe 1b
    hlt

var:
    .word 1
    .word 2
    .word 3

    .word 1
    .word 2
    .word 3

    .word 1
    .word 2
    .word 3

