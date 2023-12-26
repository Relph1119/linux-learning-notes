# ModR/M寻址方式
.text
.code16
start:
    mov $0x3f8, %dx
    mov $var1, %bx

    # 以寄存器BX的值作为内存地址，将该地址处的内容存入AL寄存器中
    mov (%bx), %al
    out %al, %dx

    # 将0x60(= 0x50 + 0x10)地址处的值，存入AL寄存器中
    mov 0x10(%bx), %al
    out %al, %dx

    # 操作数在寄存器中的寻址模式
    mov %bl, %al

    hlt

.org 0x50
var1:
    .byte 0x41

.org 0x60
var2:
    .byte 0x42