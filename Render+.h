#pragma once

#include "GameFrame.h"
#include "RGB.h"
#include "Vector2.h"
#include <vector>
#include <thread>
using namespace std;

NoReturn API_Rend(vector<vector<RGBColor>> rgbmap) {
	RGBReturnValue result[rgbmap.size() + 1][rgbmap[0].size() + 1] = {0};
	for (int x = 0; x < int(rgbmap.size()); x++) {
		for (int y = 0; y < int(rgbmap[x].size()); y++) {
			result[x][y] = RGB(rgbmap[x][y]);
		}
	}
	HDC hdc = GetDC(local_hWnd);
	HDC hdcMemory = CreateCompatibleDC(hdc);
	HBITMAP bmp = CreateCompatibleBitmap(hdc, rgbmap.size(), rgbmap[0].size());
	SelectObject(hdcMemory, bmp);
	BITMAPINFOHEADER bmpinfoheader = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	bmpinfoheader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfoheader.biWidth = rgbmap.size();
	bmpinfoheader.biHeight = rgbmap[0].size();
	bmpinfoheader.biPlanes = 1;
	bmpinfoheader.biBitCount = 32;
	bmpinfoheader.biCompression = BI_RGB;
	bmpinfoheader.biSizeImage = 0;
	bmpinfoheader.biXPelsPerMeter = 0;
	bmpinfoheader.biYPelsPerMeter = 0;
	bmpinfoheader.biClrUsed = 0;
	bmpinfoheader.biClrImportant = 0;
	BITMAPINFO bmpinfo = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	bmpinfo.bmiHeader = bmpinfoheader;
	int ReturnValue;
	ReturnValue = SetDIBits(hdc, bmp, 0, rgbmap[0].size(), result, &bmpinfo, DIB_RGB_COLORS);
	if (ReturnValue == 0 || ReturnValue == ERROR_INVALID_PARAMETER) {
		SendMessageBox("fx API_Rend Error : Invalid parameter or scan failed.", "API_Rend Error Message");
		ConsoleLog("API_Rend scan failed.", "fx API_Rend", Error);
		return;
	}
	ReturnValue = BitBlt(hdc, 0, 0, rgbmap.size(), rgbmap[0].size(), hdcMemory, 0, 0, SRCCOPY);
	if (ReturnValue == 0) {
		SendMessageBox("fx API_Rend Error : Invalid parameter or copy failed.", "API_Rend Error Message");
		ConsoleLog("API_Rend copy failed.", "fx API_Rend", Error);
		return;
	}
	DeleteObject(bmp);
	DeleteDC(hdcMemory);
	ReleaseDC(local_hWnd, hdc);
	return;
}

NoReturn RendMap(vector<vector<RGBColor>> rgbmap) {
	thread api(API_Rend, rgbmap);
	api.detach();
	return;
}
