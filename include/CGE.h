#ifndef _CGE_H_
#define _CGE_H_

extern "C" {
#define STRICT
#include <windows.h>
}
#include <iostream>

const int C_RED = 4, C_GREEN = 2, C_BLUE = 1, C_DARK = 8, C_KEEP = -1, C_WHITE = C_RED | C_GREEN | C_BLUE, C_SILVER = C_WHITE | C_DARK, C_GRAY = 0, C_BLACK = C_DARK;
const int MODE_FILL = 1, MODE_HALF = 2, MODE_EMPTY = 3, MODE_DUST = 4, MODE_DENSE = 5, MODE_KEEP = -1;

const int CTRL_CAPSLOCK	= CAPSLOCK_ON;
const int CTRL_ALT		= LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED;
const int CTRL_CTRL		= LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED;
const int CTRL_SHIFT	= SHIFT_PRESSED;

const int LEFT_BUTTON	= FROM_LEFT_1ST_BUTTON_PRESSED;
const int MID_BUTTON	= FROM_LEFT_2ND_BUTTON_PRESSED;
const int RIGHT_BUTTON	= RIGHTMOST_BUTTON_PRESSED;

const int VK_0 = 0x30, VK_1 = 0x31, VK_2 = 0x32, VK_3 = 0x33, VK_4 = 0x34, VK_5 = 0x35, VK_6 = 0x36, VK_7 = 0x37, VK_8 = 0x38, VK_9 = 0x39;
const int VK_A = 0x41, VK_B = 0x42, VK_C = 0x43, VK_D = 0x44, VK_E = 0x45, VK_F = 0x46, VK_G = 0x47, VK_H = 0x48, VK_I = 0x49, VK_J = 0x4A,
		VK_K = 0x4B, VK_L = 0x4C, VK_M = 0x4D, VK_N = 0x4E, VK_O = 0x4F, VK_P = 0x50, VK_Q = 0x51, VK_R = 0x52, VK_S = 0x53, VK_T = 0x54,
		VK_U = 0x55, VK_V = 0x56, VK_W = 0x57, VK_X = 0x58, VK_Y = 0x59, VK_Z = 0x5A;

typedef struct{
	/**fragment's coordinates, array of foreground and background color, pointer to stencil buffer value*/
	void (*fragment)(int x, int y, char[], bool *);
} FRAGMENT_SHADER;

/**x, y, button, controls*/
typedef void (*MOUSE_HANDLER)(int, int, int, int);
/**x, y, button, controls*/
typedef void (*MOUSE_MOVE_HANDLER)(int, int, int, int);
/**x, y, direction, controls*/
typedef void (*MOUSE_WHEEL_HANDLER)(int, int, int, int);
/**keydown, keycode, character, controls*/
typedef void (*KEY_HANDLER)(bool, int, char, int);

#endif
