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

/*TCHAR buf[1000];
::wsprintf(buf, L"=======> %d", CW_USEDEFAULT);
::OutputDebugString(buf);*/


/*AllocConsole();
freopen("CONOUT$", "w", stdout);
printf("i��ֵΪ%d �� %d\n", plottingAreaLength,plottingAreaWidth);
FreeConsole();*/