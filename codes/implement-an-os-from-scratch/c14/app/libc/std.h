#include "include/vesa.h"

void sleep(long ms);
void* shm_open(const char* name);
unsigned long fbmap();
void get_mode_info(struct mode_info* mode_info);