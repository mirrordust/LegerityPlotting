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