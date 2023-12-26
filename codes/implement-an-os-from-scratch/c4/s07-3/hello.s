.text
.code16
start:
    mov $0x1010, %bx
    mov %bx, %es

    mov $0x3f8, %dx
    
    # 将0x10100处存储的值0x42，存入AL寄存器中
    mov 0x100, %al
    out %al, %dx

    # 将0x10200(= 0x10100 + 0x100)处存储的值0x43，存入AL寄存器中
    mov %es:0x100, %al
    out %al, %dx

    hlt

.org 0x100
    .byte 0x42
.org 0x200
    .byte 0x43