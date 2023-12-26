#include "include/vesa.h"
#include "include/types.h"

#define BLUE 0xff0000
#define GREEN 0xff00
#define RED 0xff

void draw_zh(int origin_x, int origin_y, int index, int color, unsigned long fbbase, struct mode_info* mode_info);
void draw_en(int origin_x, int origin_y, int index, int color, unsigned long fbbase, struct mode_info* mode_info);
void draw_rect(int origin_x, int origin_y, int l, int w, int color, unsigned long fbbase, struct mode_info* mode_info);
