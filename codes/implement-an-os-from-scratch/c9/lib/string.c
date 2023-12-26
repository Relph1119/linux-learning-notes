// 填充一段内存区
void* memset(void *s, char c, unsigned long n) {
    char *tmp = s;

    while (n--) {
        *tmp++ = c;
    }

    return s;
}

/* 内存区域复制
 * @param dest 目的内存地址
 * @param src 源内存地址
 * @param n 复制的字节数
*/ 
void memcpy(void *dest, const void *src, unsigned long n) {
    char *tmp = dest;
    const char *s = src;

    // 将字节逐个从源内存的内容复制到目的内存
    while (n--) {
        *tmp++ = *s++;
    }
}