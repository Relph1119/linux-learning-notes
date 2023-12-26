#pragma once

// 如果是32位，向串口输出1次，如果是64位，向串口输出2次
#define print(x) \
do { \
  int size = sizeof(x); \
  if (size <= 4) { \
    __asm__ ("mov $0x3f8, %%dx\n\t" \
             "out %%eax, %%dx\n\t" \
             : \
             : "a"(x) \
             : "dx"); \
  } else if (size == 8) { \
    __asm__ ("mov $0x3f8, %%dx\n\t" \
             "out %%eax, %%dx\n\t" \
             "shr $32, %%rax\n\t" \
             "out %%eax, %%dx\n\t" \
             : \
             : "a"(x) \
             : "dx"); \
             } \
} while (0)