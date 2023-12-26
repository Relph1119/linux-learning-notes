// 填充一段内存区
void* memset(void *s, char c, unsigned long n) {
    char *tmp = s;

    while (n--) {
        *tmp++ = c;
    }

    return s;
}