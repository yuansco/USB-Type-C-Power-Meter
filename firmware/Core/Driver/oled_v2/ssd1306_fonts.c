
#include "ssd1306_fonts.h"

enum support_char {
	SC_SP    = 32,    // sp
	SC_DOT   = 46,    // .
	SC_0     = 48,    // 0
	SC_1     = 49,    // 1
	SC_2     = 50,    // 2
	SC_3     = 51,    // 3
	SC_4     = 52,    // 4
	SC_5     = 53,    // 5
	SC_6     = 54,    // 6
	SC_7     = 55,    // 7
	SC_8     = 56,    // 8
	SC_9     = 57,    // 9
	SC_ARR_L = 60,    // <
	SC_EQUAL = 61,    // =
	SC_ARR_R = 62,    // >
	SC_A     = 65,    // A
	SC_I     = 73,    // I
        SC_P     = 80,    // P
        SC_V     = 86,    // V
	SC_W     = 87,    // W
	SC_m     = 109,   // m
};

const int support_char_arr[] = {
	SC_SP, SC_0, SC_1, SC_2, SC_3,
	SC_4, SC_5, SC_6, SC_7, SC_8,
	SC_9, SC_A, SC_I, SC_P, SC_V,
	SC_W, SC_m, SC_DOT, SC_ARR_L,
	SC_EQUAL, SC_ARR_R
};

const int support_char_num = sizeof(support_char_arr)/sizeof(support_char_arr[0]);

static const uint16_t Font11x18_SUPPORT[] = {
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // sp
0x0000, 0x1E00, 0x3F00, 0x3300, 0x6180, 0x6180, 0x6180, 0x6D80, 0x6D80, 0x6180, 0x6180, 0x6180, 0x3300, 0x3F00, 0x1E00, 0x0000, 0x0000, 0x0000,   // 0
0x0000, 0x0600, 0x0E00, 0x1E00, 0x3600, 0x2600, 0x0600, 0x0600, 0x0600, 0x0600, 0x0600, 0x0600, 0x0600, 0x0600, 0x0600, 0x0000, 0x0000, 0x0000,   // 1
0x0000, 0x1E00, 0x3F00, 0x7380, 0x6180, 0x6180, 0x0180, 0x0300, 0x0600, 0x0C00, 0x1800, 0x3000, 0x6000, 0x7F80, 0x7F80, 0x0000, 0x0000, 0x0000,   // 2
0x0000, 0x1C00, 0x3E00, 0x6300, 0x6300, 0x0300, 0x0E00, 0x0E00, 0x0300, 0x0180, 0x0180, 0x6180, 0x7380, 0x3F00, 0x1E00, 0x0000, 0x0000, 0x0000,   // 3
0x0000, 0x0600, 0x0E00, 0x0E00, 0x1E00, 0x1E00, 0x1600, 0x3600, 0x3600, 0x6600, 0x7F80, 0x7F80, 0x0600, 0x0600, 0x0600, 0x0000, 0x0000, 0x0000,   // 4
0x0000, 0x7F00, 0x7F00, 0x6000, 0x6000, 0x6000, 0x6E00, 0x7F00, 0x6380, 0x0180, 0x0180, 0x6180, 0x7380, 0x3F00, 0x1E00, 0x0000, 0x0000, 0x0000,   // 5
0x0000, 0x1E00, 0x3F00, 0x3380, 0x6180, 0x6000, 0x6E00, 0x7F00, 0x7380, 0x6180, 0x6180, 0x6180, 0x3380, 0x3F00, 0x1E00, 0x0000, 0x0000, 0x0000,   // 6
0x0000, 0x7F80, 0x7F80, 0x0180, 0x0300, 0x0300, 0x0600, 0x0600, 0x0C00, 0x0C00, 0x0C00, 0x0800, 0x1800, 0x1800, 0x1800, 0x0000, 0x0000, 0x0000,   // 7
0x0000, 0x1E00, 0x3F00, 0x6380, 0x6180, 0x6180, 0x2100, 0x1E00, 0x3F00, 0x6180, 0x6180, 0x6180, 0x6180, 0x3F00, 0x1E00, 0x0000, 0x0000, 0x0000,   // 8
0x0000, 0x1E00, 0x3F00, 0x7300, 0x6180, 0x6180, 0x6180, 0x7380, 0x3F80, 0x1D80, 0x0180, 0x6180, 0x7300, 0x3F00, 0x1E00, 0x0000, 0x0000, 0x0000,   // 9
0x0000, 0x0E00, 0x0E00, 0x1B00, 0x1B00, 0x1B00, 0x1B00, 0x3180, 0x3180, 0x3F80, 0x3F80, 0x3180, 0x60C0, 0x60C0, 0x60C0, 0x0000, 0x0000, 0x0000,   // A
0x0000, 0x3F00, 0x3F00, 0x0C00, 0x0C00, 0x0C00, 0x0C00, 0x0C00, 0x0C00, 0x0C00, 0x0C00, 0x0C00, 0x0C00, 0x3F00, 0x3F00, 0x0000, 0x0000, 0x0000,   // I
0x0000, 0x7E00, 0x7F00, 0x6380, 0x6180, 0x6180, 0x6180, 0x6380, 0x7F00, 0x7E00, 0x6000, 0x6000, 0x6000, 0x6000, 0x6000, 0x0000, 0x0000, 0x0000,   // P
0x0000, 0x60C0, 0x60C0, 0x60C0, 0x3180, 0x3180, 0x3180, 0x1B00, 0x1B00, 0x1B00, 0x1B00, 0x0E00, 0x0E00, 0x0E00, 0x0400, 0x0000, 0x0000, 0x0000,   // V
0x0000, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xCCC0, 0x4C80, 0x4C80, 0x5E80, 0x5280, 0x5280, 0x7380, 0x6180, 0x6180, 0x0000, 0x0000, 0x0000,   // W
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDD80, 0xFFC0, 0xCEC0, 0xCCC0, 0xCCC0, 0xCCC0, 0xCCC0, 0xCCC0, 0xCCC0, 0xCCC0, 0x0000, 0x0000, 0x0000,   // m
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0C00, 0x0C00, 0x0000, 0x0000, 0x0000,   // .
/*
0x0000, 0x0000, 0x0000, 0x0000, 0x0080, 0x0380, 0x0E00, 0x3800, 0x6000, 0x3800, 0x0E00, 0x0380, 0x0080, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // <
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7F80, 0x7F80, 0x0000, 0x0000, 0x7F80, 0x7F80, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // =
0x0000, 0x0000, 0x0000, 0x0000, 0x4000, 0x7000, 0x1C00, 0x0700, 0x0180, 0x0700, 0x1C00, 0x7000, 0x4000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // >
*/
0x0000, 0x0000, 0x0060, 0x00e0, 0x01e0, 0x07e0, 0x1fe0, 0x7fe0, 0xffe0, 0xffe0, 0x7fe0, 0x1fe0, 0x07e0, 0x01e0, 0x00e0, 0x0060, 0x0000, 0x0000,   // new <
0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,   // new =
0x0000, 0x0000, 0xc000, 0xe000, 0xf000, 0xfc00, 0xff00, 0xffc0, 0xffe0, 0xffe0, 0xffc0, 0xff00, 0xfc00, 0xf000, 0xe000, 0xc000, 0x0000, 0x0000,   // new >
};

FontDef Font_11x18_SUPPORT = {11, 18, Font11x18_SUPPORT};