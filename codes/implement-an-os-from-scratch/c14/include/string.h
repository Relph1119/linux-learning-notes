#pragma once

#include "include/types.h"

void* memset(void *s, char c, unsigned long n);
void memcpy(void *dest, const void *src, unsigned long n);
int strcmp(const char *s1, const char *s2);
int strlen(const char *s);