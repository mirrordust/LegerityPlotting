#pragma once

#include "stdafx.h"

using namespace std;

struct calres
{
	double answer;
	bool valid;
};


void plot(HDC hdc, string expression, POINT origin,
	float XrangeLeft, float XrangeRight, float XplottingScale, float YplottingScale);

queue<string> postfix(string expression);

int judgeX(char ch);

calres calculate(queue<string> rpn, float x);

COLORREF cscolor(HWND hButton, COLORREF pre);

bool SaveBMP(BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile);

BYTE* ConvertRGBToBMPBuffer(BYTE* Buffer, int width, int height, long* newsize);

BOOL SelectDir(HWND hWnd, wstring &strPath);

BOOL SaveAsBmp(HWND hwnd, wstring &strPath);

/*TCHAR buf[1000];
::wsprintf(buf, L"=======> %d", CW_USEDEFAULT);
::OutputDebugString(buf);*/


/*AllocConsole();
freopen("CONOUT$", "w", stdout);
printf("i的值为%d 和 %d\n", plottingAreaLength,plottingAreaWidth);
FreeConsole();*/


/*HDC hdc_bg = GetDC(button_bg);
HBRUSH hBrush_bg = CreateSolidBrush(RGB(0,0,0));
SelectObject(hdc_bg, hBrush_bg);
RECT rect_bg;
GetClientRect(button_bg, &rect_bg);
FillRect(hdc_bg, &rect_bg, hBrush_bg);*/


/*HWND cHwnd = GetDlgItem(console, IDC_BUTTON_BG);
HDC cHdc = GetDC(cHwnd);
HBRUSH hBrush = CreateSolidBrush(numberColor);
SelectObject(cHdc, hBrush);
RECT rect_bg;
GetClientRect(cHwnd, &rect_bg);
FillRect(cHdc, &rect_bg, hBrush);*/