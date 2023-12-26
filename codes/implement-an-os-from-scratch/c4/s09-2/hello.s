# 直接寻址
.text
.code16
start:
    mov var, %al
    mov $0x3f8, %dx
    out %al, %dx
    hlt

.org 0x20
var:
    .byte 0x41