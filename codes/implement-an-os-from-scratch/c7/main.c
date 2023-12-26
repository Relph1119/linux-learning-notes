int main() {
    // 输出字符M
    __asm__ ("mov $0x3f8, %dx\n\t"
             "mov $'M', %ax\n\t"
             "out %ax, %dx\n"
    );
    __asm__ ("hlt");
}