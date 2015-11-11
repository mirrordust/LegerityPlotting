#pragma once

#include "stdafx.h"

using namespace std;

void plot(HDC hdc, COLORREF color, string expression, POINT origin,
	float XrangeLeft, float XrangeRight, float XplottingScale, float YplottingScale);

queue<string> postfix(string expression);

int judgeX(char ch);

double calculate(queue<string> rpn, float x);
