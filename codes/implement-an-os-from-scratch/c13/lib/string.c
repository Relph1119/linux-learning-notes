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

/* 字符串比较
 * @param s1 字符串1
 * @param s2 字符串2
 * @return 如果两个字符串的全部字符都相同，返回0。
 *         如果字符串1大于字符串2，返回1，否则返回-1。
*/
int strcmp(const char *s1, const char *s2) {
    unsigned char c1, c2;

    while (1) {
        c1 = *s1++;
        c2 = *s2++;
        if (c1 != c2) {
            return c1 < c2 ? -1 : 1;
        }
        if (!c1) {
            break;
        }
    }

    return 0;
}

/* 统计字符串长度
 * @param s 字符串
 * @return 返回字符串长度
*/
int strlen(const char *s) {
  const char *sc;

  for (sc = s; *sc != '\0'; ++sc);

  return sc - s;
}