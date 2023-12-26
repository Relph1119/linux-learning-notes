#include "include/print.h"

// 使用键码集2
unsigned char keymap[256] = {
    [0x1c] = 'a',
    [0x32] = 'b',
    [0x21] = 'c',
    [0x23] = 'd',
    [0x24] = 'e',
    [0x2b] = 'f',
    [0x34] = 'g',
    [0x33] = 'h',
    [0x43] = 'i',
    [0x3b] = 'j',
    [0x42] = 'k',
    [0x4b] = 'l',
    [0x3a] = 'm',
    [0x31] = 'n',
    [0x44] = 'o',
    [0x4d] = 'p',
    [0x15] = 'q',
    [0x2d] = 'r',
    [0x1b] = 's',
    [0x2c] = 't',
    [0x3c] = 'u',
    [0x2a] = 'v',
    [0x1d] = 'w',
    [0x22] = 'x',
    [0x35] = 'y',
    [0x1a] = 'z',
    [0x45] = '0',
    [0x16] = '1',
    [0x1e] = '2',
    [0x26] = '3',
    [0x25] = '4',
    [0x2e] = '5',
    [0x36] = '6',
    [0x3d] = '7',
    [0x3e] = '8',
    [0x46] = '9'
};

// 键盘中断处理函数
void process_kb() {
    // 当前按键
    unsigned char scancode;
    // 前置修饰符
    static unsigned char prevcode;

    // 从端口0x60读取键盘控制器扫描码
    __asm__ ("inb $0x60, %%al" : "=a"(scancode));
    
    // 由于不适用扩展键位，可以使用0xE0进行判断
    // 记录前置修饰符
    if (scancode == 0xe0 || scancode == 0xf0) {
        prevcode = scancode;
        return;
    }

    // 如果前置修饰符是F0，则表示释放操作
    if (prevcode == 0xe0 || prevcode == 0xf0) {
        prevcode = 0;
        return;
    }
    
    // 如果前置修饰符不是F0，则表示按键操作，打印键值
    print(keymap[scancode]);	
}
