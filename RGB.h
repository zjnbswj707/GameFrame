#pragma once

#include <windows.h>

#undef RGB

struct struct_RGBColor {
	int red;
	int green;
	int blue;
};
typedef struct struct_RGBColor RGBColor;
typedef unsigned long int RGBReturnValue;
RGBReturnValue RGB(RGBColor color) {
	return (COLORREF)(((BYTE)(color.red) | ((WORD)((BYTE)(color.green)) << 8)) | (((DWORD)(BYTE)(color.blue)) << 16));
}
