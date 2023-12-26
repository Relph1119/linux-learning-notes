# SIB寻址
.text
.code16
start:
    # EBX寄存器存储数组基址 
    mov $var, %ebx
    # ESI寄存器存储数组的索引
    mov $0, %esi
    mov $0, %ax

1:
    # 将数组的元素累加到AX寄存器中
    add (%ebx, %esi, 2), %ax
    # 指向数组的下一个元素
    add $1, %esi
    # 循环5个数
    cmp $5, %esi
    jb 1b
    hlt

var:
    .word 1
    .word 2
    .word 3
    .word 4
    .word 5