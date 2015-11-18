#pragma once

#include "stdafx.h"

using namespace std;

struct calres
{
	double answer;
	bool valid;
};

struct valsym
{
	INT type;// 0 ���� ;1 x ;-1 �����
	double val;
	TCHAR sym[7];
};

void plot(HDC hdc, TCHAR* expression, POINT origin,
	float XrangeLeft, float XrangeRight, float XplottingScale, float YplottingScale);

queue<valsym> postfix(TCHAR* expression);

int judgeX(TCHAR ch);

int judgeBuf(TCHAR* buf);

calres calculate(queue<valsym> rpn, float x);

TCHAR** tokenizer(TCHAR* expression);

/*TCHAR buf[1000];
::wsprintf(buf, L"=======> %d", CW_USEDEFAULT);
::OutputDebugString(buf);*/


/*AllocConsole();
freopen("CONOUT$", "w", stdout);
printf("i��ֵΪ%d �� %d\n", plottingAreaLength,plottingAreaWidth);
FreeConsole();*/