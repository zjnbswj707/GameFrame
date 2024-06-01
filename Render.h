#pragma once

#include "GameFrame.h"
#include "RGB.h"
#include "Vector2.h"
#include <vector>
#include <pthread.h>
using namespace std;


typedef void RendPixelReturnValue;
RendPixelReturnValue RendPixel(RGBColor rgb, Vector2 pos) {
	HDC hdc = GetDC(local_hWnd);
	SetPixel(hdc, pos.x, pos.y, RGB(rgb));
	ReleaseDC(local_hWnd, hdc);
	return;
}

CRITICAL_SECTION g_cs;
const int THREAD_COUNT = 8;

typedef struct struct_RendParam {
    vector<vector<RGBColor>>* pRgbmap;
    int startX, startY, endX, endY;
} RendParam;

DWORD WINAPI ThreadProc(LPVOID lpParam) {
    RendParam* param = (RendParam*)lpParam;
    vector<vector<RGBColor>>& rgbmap = *(param->pRgbmap);
    for (int y = param->startY; y < param->endY; y++) {
        for (int x = param->startX; x < param->endX; x++) {
            EnterCriticalSection(&g_cs);
            RendPixel(rgbmap[y][x], { x, y });
            LeaveCriticalSection(&g_cs);
        }
    }
    delete param;
    return 0;
}

APIReturnValue API_Rend(vector<vector<RGBColor>> rgbmap) {
    InitializeCriticalSection(&g_cs);
    int width = rgbmap.size();
    int height = rgbmap.size();
    int stepX = width / THREAD_COUNT;
    int remainX = width % THREAD_COUNT;
    vector<RendParam*> params;
    for (int i = 0; i < THREAD_COUNT; i++) {
        RendParam* param = new RendParam;
        param->pRgbmap = &rgbmap;
        param->startX = i * stepX;
        param->startY = 0;
        param->endX = (i + 1) * stepX + (i == THREAD_COUNT - 1 ? remainX : 0);
        param->endY = height;
        params.push_back(param);
    }
    vector<HANDLE> threads(THREAD_COUNT);
    for (int i = 0; i < THREAD_COUNT; i++) {
        threads[i] = CreateThread(NULL, 0, ThreadProc, params[i], 0, NULL);
    }
    WaitForMultipleObjects(THREAD_COUNT, reinterpret_cast<void* const*>(threads.data()), TRUE, INFINITE);
    for (int i = 0; i < THREAD_COUNT; i++) {
        CloseHandle(threads[i]);
    }
    DeleteCriticalSection(&g_cs);
}

typedef void RendMapReturnValue;
RendMapReturnValue RendMap(vector<vector<RGBColor>> rgbmap) {
	API_Rend(rgbmap);
	return;
}
