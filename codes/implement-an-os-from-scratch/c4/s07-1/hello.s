.text
.code16
start:
    # 向串口输出一个字符A
    mov $'A', %al
    # 准备out指令的目的操作数
    mov $0x3f8, %dx
    # 向串口发起写操作
    out %al, %dx
    # 告诉处理器停止执行
    hlt