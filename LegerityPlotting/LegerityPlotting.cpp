// LegerityPlotting.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "LegerityPlotting.h"


#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

BOOL XaxisOn; //是否显示X轴
BOOL YaxisOn; //是否显示Y轴
BOOL gridOn; //是否显示格子线
BOOL tickMarksOn; //是否显示刻度线
BOOL numbersOn; //是否显示坐标轴上的数字

COLORREF backgroundColor; //背景颜色
COLORREF lineColor; //图形线条颜色
INT XtickDistance; //X刻度隔几个刻度画一条刻度线
INT YtickDistance; //Y刻度隔几个刻度画一条刻度线
INT XlabelInterval; //X轴隔几个刻度线标数字
INT YlabelInterval; //Y轴隔几个刻度线标数字
FLOAT XrangeLeft; //X左范围
FLOAT XrangeRight; //X右范围
FLOAT YrangeTop; //Y上范围
FLOAT YrangeBottom; //Y下范围
FLOAT XplottingScale; //X比例尺（实际/逻辑）
FLOAT YplottingScale; //Y比例尺（实际/逻辑）
FLOAT tickMarkLength; //刻度线长度
FLOAT gridSpacing; //格子间距


// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LEGERITYPLOTTING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	XaxisOn = TRUE; //是否显示X轴
	YaxisOn = TRUE; //是否显示Y轴
	gridOn = TRUE; //是否显示格子线
	tickMarksOn = TRUE; //是否显示刻度线
	numbersOn = TRUE; //是否显示坐标轴上的数字

	backgroundColor = RGB(255, 0, 0); //背景颜色
	lineColor = RGB(0, 0, 0); //图形线条颜色

	XtickDistance = 1; //X刻度隔几个刻度画一条刻度线
	YtickDistance = 1; //Y刻度隔几个刻度画一条刻度线
	XlabelInterval = 5; //X轴隔几个刻度线标数字
	YlabelInterval = 5; //Y轴隔几个刻度线标数字
	XplottingScale = 30; //X比例尺（实际/逻辑）
	YplottingScale = 30; //Y比例尺（实际/逻辑）
	tickMarkLength = 20; //刻度线长度
	gridSpacing = 2; //格子间距

	//X和Y的范围 0表示未设置 转而根据窗口大小决定
	XrangeLeft = 0; //X左范围
	XrangeRight = 0; //X右范围
	YrangeTop = 0; //Y上范围
	YrangeBottom = 0; //Y下范围


	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LEGERITYPLOTTING));

	MSG msg;

	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LEGERITYPLOTTING));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LEGERITYPLOTTING);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		//RECT plottingAreaRect;
		//INT plottingAreaLength;
		//INT plottingAreaWidth;
		//GetClientRect(hWnd, &plottingAreaRect);
		//plottingAreaLength = plottingAreaRect.right - plottingAreaRect.left;
		//plottingAreaWidth = plottingAreaRect.bottom - plottingAreaRect.top;

		//HDC hdc = GetDC(hWnd);
		//HDC hMen = CreateCompatibleDC(hdc);
		//HBITMAP hBmp = CreateCompatibleBitmap(hdc, plottingAreaLength, plottingAreaWidth);
		//SelectObject(hMen, hBmp);

		//ReleaseDC(hWnd, hdc);

		//
		//POINT origin;
		//

		//HPEN hpen1 = ::CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		//HPEN hpenOld = (HPEN)::SelectObject(hMen, hpen1);


		//if (XrangeLeft == 0 && XrangeRight == 0
		//	&& YrangeTop == 0 && YrangeBottom == 0)
		//{//未设置X和Y的的范围，则默认
		//	XrangeRight = plottingAreaLength / XplottingScale / 2;
		//	XrangeLeft = -XrangeRight;
		//	YrangeBottom = plottingAreaWidth / YplottingScale / 2;
		//	YrangeTop = -YrangeBottom;
		//}


		////确定坐标原点
		////origin.x = plottingAreaRect.right - XrangeRight * XplottingScale;
		////origin.y = plottingAreaRect.bottom - YrangeBottom * YplottingScale;
		//origin.x = plottingAreaLength / 2;
		//origin.y = plottingAreaWidth / 2;

		//// 开始绘图
		////SetBkColor(hdc, backgroundColor);


		//if (gridOn == TRUE)
		//{
		//	HPEN hpen2 = ::CreatePen(PS_DASH, 1, RGB(90, 90, 90));
		//	HPEN hpen3 = ::CreatePen(PS_SOLID, 2, RGB(90, 90, 90));
		//	hpenOld = (HPEN)::SelectObject(hMen, hpen2);
		//	INT j = 0;
		//	//X轴
		//	for (FLOAT i = origin.x - XtickDistance * XplottingScale; i > plottingAreaRect.left;
		//	i -= XtickDistance * XplottingScale)
		//	{
		//		j++;
		//		if (j % XlabelInterval == 0)
		//		{
		//			hpenOld = (HPEN)::SelectObject(hMen, hpen3);
		//			MoveToEx(hMen, i, plottingAreaRect.top, NULL);
		//			LineTo(hMen, i, plottingAreaRect.bottom);
		//		}
		//		else
		//		{
		//			hpenOld = (HPEN)::SelectObject(hMen, hpen2);
		//			MoveToEx(hMen, i, plottingAreaRect.top, NULL);
		//			LineTo(hMen, i, plottingAreaRect.bottom);
		//		}

		//	}
		//	j = 0;
		//	for (FLOAT i = origin.x + XtickDistance * XplottingScale; i < plottingAreaRect.right;
		//	i += XtickDistance * XplottingScale)
		//	{
		//		j++;
		//		if (j % XlabelInterval == 0)
		//		{
		//			hpenOld = (HPEN)::SelectObject(hMen, hpen3);
		//			MoveToEx(hMen, i, plottingAreaRect.top, NULL);
		//			LineTo(hMen, i, plottingAreaRect.bottom);
		//		}
		//		else
		//		{
		//			hpenOld = (HPEN)::SelectObject(hMen, hpen2);
		//			MoveToEx(hMen, i, plottingAreaRect.top, NULL);
		//			LineTo(hMen, i, plottingAreaRect.bottom);
		//		}
		//	}
		//	j = 0;
		//	//Y轴
		//	for (FLOAT i = origin.y - YtickDistance * YplottingScale; i > plottingAreaRect.top;
		//	i -= YtickDistance * YplottingScale)
		//	{
		//		j++;
		//		if (j % YlabelInterval == 0)
		//		{
		//			hpenOld = (HPEN)::SelectObject(hMen, hpen3);
		//			MoveToEx(hMen, plottingAreaRect.left, i, NULL);
		//			LineTo(hMen, plottingAreaRect.right, i);
		//		}
		//		else
		//		{
		//			hpenOld = (HPEN)::SelectObject(hMen, hpen2);
		//			MoveToEx(hMen, plottingAreaRect.left, i, NULL);
		//			LineTo(hMen, plottingAreaRect.right, i);
		//		}
		//	}
		//	j = 0;
		//	for (FLOAT i = origin.y + YtickDistance * YplottingScale; i < plottingAreaRect.bottom;
		//	i += YtickDistance * YplottingScale)
		//	{
		//		j++;
		//		if (j % YlabelInterval == 0)
		//		{
		//			hpenOld = (HPEN)::SelectObject(hMen, hpen3);
		//			MoveToEx(hMen, plottingAreaRect.left, i, NULL);
		//			LineTo(hMen, plottingAreaRect.right, i);
		//		}
		//		else
		//		{
		//			hpenOld = (HPEN)::SelectObject(hMen, hpen2);
		//			MoveToEx(hMen, plottingAreaRect.left, i, NULL);
		//			LineTo(hMen, plottingAreaRect.right, i);
		//		}
		//	}
		//	::DeleteObject(hpen2);
		//	::DeleteObject(hpen3);
		//}
		//hpenOld = (HPEN)::SelectObject(hMen, hpen1);

		//if (XaxisOn == TRUE)
		//{
		//	MoveToEx(hMen, 0, origin.y, NULL);
		//	LineTo(hMen, plottingAreaRect.right, origin.y);
		//}
		//if (YaxisOn == TRUE)
		//{
		//	MoveToEx(hMen, origin.x, 0, NULL);
		//	LineTo(hMen, origin.x, plottingAreaRect.bottom);
		//}

		//if (tickMarksOn == TRUE)
		//{
		//	INT j = 0;
		//	//X轴刻度线
		//	for (FLOAT i = origin.x - XtickDistance * XplottingScale; i > plottingAreaRect.left;
		//	i -= XtickDistance * XplottingScale)
		//	{
		//		j++;
		//		if (j % XlabelInterval == 0)
		//		{
		//			MoveToEx(hMen, i, origin.y - tickMarkLength / 2, NULL);
		//			LineTo(hMen, i, origin.y + tickMarkLength / 2);
		//		}
		//		else
		//		{
		//			MoveToEx(hMen, i, origin.y - tickMarkLength / 4, NULL);
		//			LineTo(hMen, i, origin.y + tickMarkLength / 4);
		//		}
		//	}
		//	j = 0;
		//	for (FLOAT i = origin.x + XtickDistance * XplottingScale; i < plottingAreaRect.right;
		//	i += XtickDistance * XplottingScale)
		//	{
		//		j++;
		//		if (j % XlabelInterval == 0)
		//		{
		//			MoveToEx(hMen, i, origin.y - tickMarkLength / 2, NULL);
		//			LineTo(hMen, i, origin.y + tickMarkLength / 2);
		//		}
		//		else
		//		{
		//			MoveToEx(hMen, i, origin.y - tickMarkLength / 4, NULL);
		//			LineTo(hMen, i, origin.y + tickMarkLength / 4);
		//		}
		//	}
		//	j = 0;
		//	//Y轴刻度线
		//	for (FLOAT i = origin.y - YtickDistance * YplottingScale; i > plottingAreaRect.top;
		//	i -= YtickDistance * YplottingScale)
		//	{
		//		j++;
		//		if (j % YlabelInterval == 0)
		//		{
		//			MoveToEx(hMen, origin.x - tickMarkLength / 2, i, NULL);
		//			LineTo(hMen, origin.x + tickMarkLength / 2, i);
		//		}
		//		else
		//		{
		//			MoveToEx(hMen, origin.x - tickMarkLength / 4, i, NULL);
		//			LineTo(hMen, origin.x + tickMarkLength / 4, i);
		//		}
		//	}
		//	j = 0;
		//	for (FLOAT i = origin.y + YtickDistance * YplottingScale; i < plottingAreaRect.bottom;
		//	i += YtickDistance * YplottingScale)
		//	{
		//		j++;
		//		if (j % YlabelInterval == 0)
		//		{
		//			MoveToEx(hMen, origin.x - tickMarkLength / 2, i, NULL);
		//			LineTo(hMen, origin.x + tickMarkLength / 2, i);
		//		}
		//		else
		//		{
		//			MoveToEx(hMen, origin.x - tickMarkLength / 4, i, NULL);
		//			LineTo(hMen, origin.x + tickMarkLength / 4, i);
		//		}
		//	}
		//}

		//if (numbersOn == TRUE)
		//{
		//	TCHAR buf[20];
		//	INT j = 0;
		//	//X轴数字
		//	for (FLOAT i = origin.x - XtickDistance * XplottingScale; i > plottingAreaRect.left;
		//	i -= XtickDistance * XplottingScale)
		//	{
		//		j--;
		//		if (j % XlabelInterval == 0)
		//		{
		//			::wsprintf(buf, L"%d", j);
		//			SetTextAlign(hMen, TA_CENTER);
		//			TextOut(hMen, i - wcslen(buf) / 2, origin.y + tickMarkLength / 2, buf, ::wcslen(buf));
		//		}
		//	}
		//	j = 0;
		//	for (FLOAT i = origin.x + XtickDistance * XplottingScale; i < plottingAreaRect.right;
		//	i += XtickDistance * XplottingScale)
		//	{
		//		j++;
		//		if (j % XlabelInterval == 0)
		//		{
		//			::wsprintf(buf, L"%d", j);
		//			TextOut(hMen, i - wcslen(buf) / 2, origin.y + tickMarkLength / 2, buf, ::wcslen(buf));
		//		}
		//	}
		//	j = 0;
		//	SetTextAlign(hMen, TA_BASELINE + TA_RIGHT);
		//	//Y轴数字
		//	for (FLOAT i = origin.y - YtickDistance * YplottingScale; i > plottingAreaRect.top;
		//	i -= YtickDistance * YplottingScale)
		//	{
		//		j++;
		//		if (j % YlabelInterval == 0)
		//		{
		//			::wsprintf(buf, L"%d", j);
		//			TextOut(hMen, origin.x - tickMarkLength / 2 - wcslen(buf), i, buf, ::wcslen(buf));
		//		}
		//	}
		//	j = 0;
		//	for (FLOAT i = origin.y + YtickDistance * YplottingScale; i < plottingAreaRect.bottom;
		//	i += YtickDistance * YplottingScale)
		//	{
		//		j--;
		//		if (j % YlabelInterval == 0)
		//		{
		//			::wsprintf(buf, L"%d", j);
		//			TextOut(hMen, origin.x - tickMarkLength / 2 - wcslen(buf), i, buf, ::wcslen(buf));
		//		}
		//	}
		//}

		////画函数图象
		//HPEN hpen4 = ::CreatePen(PS_DASH, 2, RGB(250, 0, 0));
		//hpenOld = (HPEN)::SelectObject(hMen, hpen4);
		//FLOAT x, y;
		//for (FLOAT i = XrangeLeft * XplottingScale; i < XrangeRight * XplottingScale; i += 0.01) // x每增长0.01绘制1个点
		//{
		//	x = origin.x + i;
		//	y = origin.y - tan(i / XplottingScale) * YplottingScale;
		//	// 在 (x,y) 处绘制一个点
		//	SetPixel(hMen, x, y, RGB(250, 0, 0));
		//}

		////以上为画函数图象


		////Rectangle(hdc, 10, 10, 100, 100);

		//SelectObject(hMen, hpenOld);
		////SelectObject(hdc, hbrOld);

		//::DeleteObject(hpen1);
		////::DeleteObject(hbr);

		//PAINTSTRUCT ps;
		//HDC hdc2 = BeginPaint(hWnd, &ps);

		//// TODO: 在此处添加使用 hdc 的任何绘图代码...
		//BitBlt(hdc, 0, 0, plottingAreaLength, plottingAreaWidth, hMen, 0, 0, SRCCOPY);

		//EndPaint(hWnd, &ps);
		//=========================================================================
		PAINTSTRUCT ps;
		RECT plottingAreaRect;
		POINT origin;
		HDC hdc = BeginPaint(hWnd, &ps);
		INT plottingAreaLength;
		INT plottingAreaWidth;
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		HPEN hpen1 = ::CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		HPEN hpenOld = (HPEN)::SelectObject(hdc, hpen1);

		//HBRUSH hbr = ::CreateSolidBrush(0xabcdef);
		//HBRUSH hbrOld = (HBRUSH)::SelectObject(hdc, hbr);
		GetClientRect(hWnd, &plottingAreaRect);
		plottingAreaLength = plottingAreaRect.right - plottingAreaRect.left;
		plottingAreaWidth = plottingAreaRect.bottom - plottingAreaRect.top;

		/*AllocConsole();
		freopen("CONOUT$", "w", stdout);
		printf("i的值为%d 和 %d\n", plottingAreaLength,plottingAreaWidth);
		FreeConsole();*/

		if (XrangeLeft == 0 && XrangeRight == 0
			&& YrangeTop == 0 && YrangeBottom == 0)
		{//未设置X和Y的的范围，则默认
			XrangeRight = plottingAreaLength / XplottingScale / 2;
			XrangeLeft = -XrangeRight;
			YrangeBottom = plottingAreaWidth / YplottingScale / 2;
			YrangeTop = -YrangeBottom;
		}

		/*XrangeRight = 20;
		XrangeLeft = -7;
		YrangeBottom = 13;
		YrangeTop = -16;
		XplottingScale = plottingAreaLength / (XrangeRight - XrangeLeft);
		YplottingScale = plottingAreaWidth / (YrangeBottom - YrangeTop);*/

		//确定坐标原点
		//origin.x = plottingAreaRect.right - XrangeRight * XplottingScale;
		//origin.y = plottingAreaRect.bottom - YrangeBottom * YplottingScale;
		origin.x = plottingAreaLength / 2;
		origin.y = plottingAreaWidth / 2;

		// 开始绘图
		//SetBkColor(hdc, backgroundColor);

		
		if (gridOn == TRUE)
		{
			HPEN hpen2 = ::CreatePen(PS_DASH, 1, RGB(90, 90, 90));
			HPEN hpen3 = ::CreatePen(PS_SOLID, 2, RGB(90, 90, 90));
			hpenOld = (HPEN)::SelectObject(hdc, hpen2);
			INT j = 0;
			//X轴
			for (FLOAT i = origin.x - XtickDistance * XplottingScale; i > plottingAreaRect.left;
			i -= XtickDistance * XplottingScale)
			{
				j++;
				if (j % XlabelInterval == 0)
				{
					hpenOld = (HPEN)::SelectObject(hdc, hpen3);
					MoveToEx(hdc, i, plottingAreaRect.top, NULL);
					LineTo(hdc, i, plottingAreaRect.bottom);
				}
				else
				{
					hpenOld = (HPEN)::SelectObject(hdc, hpen2);
					MoveToEx(hdc, i, plottingAreaRect.top, NULL);
					LineTo(hdc, i, plottingAreaRect.bottom);
				}

			}
			j = 0;
			for (FLOAT i = origin.x + XtickDistance * XplottingScale; i < plottingAreaRect.right;
			i += XtickDistance * XplottingScale)
			{
				j++;
				if (j % XlabelInterval == 0)
				{
					hpenOld = (HPEN)::SelectObject(hdc, hpen3);
					MoveToEx(hdc, i, plottingAreaRect.top, NULL);
					LineTo(hdc, i, plottingAreaRect.bottom);
				}
				else
				{
					hpenOld = (HPEN)::SelectObject(hdc, hpen2);
					MoveToEx(hdc, i, plottingAreaRect.top, NULL);
					LineTo(hdc, i, plottingAreaRect.bottom);
				}
			}
			j = 0;
			//Y轴
			for (FLOAT i = origin.y - YtickDistance * YplottingScale; i > plottingAreaRect.top;
			i -= YtickDistance * YplottingScale)
			{
				j++;
				if (j % YlabelInterval == 0)
				{
					hpenOld = (HPEN)::SelectObject(hdc, hpen3);
					MoveToEx(hdc, plottingAreaRect.left, i, NULL);
					LineTo(hdc, plottingAreaRect.right, i);
				}
				else
				{
					hpenOld = (HPEN)::SelectObject(hdc, hpen2);
					MoveToEx(hdc, plottingAreaRect.left, i, NULL);
					LineTo(hdc, plottingAreaRect.right, i);
				}
			}
			j = 0;
			for (FLOAT i = origin.y + YtickDistance * YplottingScale; i < plottingAreaRect.bottom;
			i += YtickDistance * YplottingScale)
			{
				j++;
				if (j % YlabelInterval == 0)
				{
					hpenOld = (HPEN)::SelectObject(hdc, hpen3);
					MoveToEx(hdc, plottingAreaRect.left, i, NULL);
					LineTo(hdc, plottingAreaRect.right, i);
				}
				else
				{
					hpenOld = (HPEN)::SelectObject(hdc, hpen2);
					MoveToEx(hdc, plottingAreaRect.left, i, NULL);
					LineTo(hdc, plottingAreaRect.right, i);
				}
			}
			::DeleteObject(hpen2);
			::DeleteObject(hpen3);
		}
		hpenOld = (HPEN)::SelectObject(hdc, hpen1);

		if (XaxisOn == TRUE)
		{
			MoveToEx(hdc, 0, origin.y, NULL);
			LineTo(hdc, plottingAreaRect.right, origin.y);
		}
		if (YaxisOn == TRUE)
		{
			MoveToEx(hdc, origin.x, 0, NULL);
			LineTo(hdc, origin.x, plottingAreaRect.bottom);
		}

		if (tickMarksOn == TRUE)
		{
			INT j = 0;
			//X轴刻度线
			for (FLOAT i = origin.x - XtickDistance * XplottingScale; i > plottingAreaRect.left;
			i -= XtickDistance * XplottingScale)
			{
				j++;
				if (j % XlabelInterval == 0)
				{
					MoveToEx(hdc, i, origin.y - tickMarkLength / 2, NULL);
					LineTo(hdc, i, origin.y + tickMarkLength / 2);
				}
				else
				{
					MoveToEx(hdc, i, origin.y - tickMarkLength / 4, NULL);
					LineTo(hdc, i, origin.y + tickMarkLength / 4);
				}
			}
			j = 0;
			for (FLOAT i = origin.x + XtickDistance * XplottingScale; i < plottingAreaRect.right;
			i += XtickDistance * XplottingScale)
			{
				j++;
				if (j % XlabelInterval == 0)
				{
					MoveToEx(hdc, i, origin.y - tickMarkLength / 2, NULL);
					LineTo(hdc, i, origin.y + tickMarkLength / 2);
				}
				else
				{
					MoveToEx(hdc, i, origin.y - tickMarkLength / 4, NULL);
					LineTo(hdc, i, origin.y + tickMarkLength / 4);
				}
			}
			j = 0;
			//Y轴刻度线
			for (FLOAT i = origin.y - YtickDistance * YplottingScale; i > plottingAreaRect.top;
			i -= YtickDistance * YplottingScale)
			{
				j++;
				if (j % YlabelInterval == 0)
				{
					MoveToEx(hdc, origin.x - tickMarkLength / 2, i, NULL);
					LineTo(hdc, origin.x + tickMarkLength / 2, i);
				}
				else
				{
					MoveToEx(hdc, origin.x - tickMarkLength / 4, i, NULL);
					LineTo(hdc, origin.x + tickMarkLength / 4, i);
				}
			}
			j = 0;
			for (FLOAT i = origin.y + YtickDistance * YplottingScale; i < plottingAreaRect.bottom;
			i += YtickDistance * YplottingScale)
			{
				j++;
				if (j % YlabelInterval == 0)
				{
					MoveToEx(hdc, origin.x - tickMarkLength / 2, i, NULL);
					LineTo(hdc, origin.x + tickMarkLength / 2, i);
				}
				else
				{
					MoveToEx(hdc, origin.x - tickMarkLength / 4, i, NULL);
					LineTo(hdc, origin.x + tickMarkLength / 4, i);
				}
			}
		}
		
		if (numbersOn == TRUE)
		{
			TCHAR buf[20];
			INT j = 0;
			//X轴数字
			for (FLOAT i = origin.x - XtickDistance * XplottingScale; i > plottingAreaRect.left;
			i -= XtickDistance * XplottingScale)
			{
				j--;
				if (j % XlabelInterval == 0)
				{
					::wsprintf(buf, L"%d", j);
					SetTextAlign(hdc, TA_CENTER);
					TextOut(hdc, i - wcslen(buf) / 2, origin.y + tickMarkLength / 2, buf, ::wcslen(buf));
				}
			}
			j = 0;
			for (FLOAT i = origin.x + XtickDistance * XplottingScale; i < plottingAreaRect.right;
			i += XtickDistance * XplottingScale)
			{
				j++;
				if (j % XlabelInterval == 0)
				{
					::wsprintf(buf, L"%d", j);
					TextOut(hdc, i - wcslen(buf) / 2, origin.y + tickMarkLength / 2, buf, ::wcslen(buf));
				}
			}
			j = 0;
			SetTextAlign(hdc, TA_BASELINE + TA_RIGHT);
			//Y轴数字
			for (FLOAT i = origin.y - YtickDistance * YplottingScale; i > plottingAreaRect.top;
			i -= YtickDistance * YplottingScale)
			{
				j++;
				if (j % YlabelInterval == 0)
				{
					::wsprintf(buf, L"%d", j);
					TextOut(hdc, origin.x - tickMarkLength / 2 - wcslen(buf), i, buf, ::wcslen(buf));
				}
			}
			j = 0;
			for (FLOAT i = origin.y + YtickDistance * YplottingScale; i < plottingAreaRect.bottom;
			i += YtickDistance * YplottingScale)
			{
				j--;
				if (j % YlabelInterval == 0)
				{
					::wsprintf(buf, L"%d", j);
					TextOut(hdc, origin.x - tickMarkLength / 2 - wcslen(buf), i, buf, ::wcslen(buf));
				}
			}
		}

		//画函数图象
		HPEN hpen4 = ::CreatePen(PS_DASH, 2, RGB(250, 0, 0));
		hpenOld = (HPEN)::SelectObject(hdc, hpen4);
		plot(hdc, RGB(250,0,0), "3^x", origin, 
			XrangeLeft, XrangeRight, XplottingScale, YplottingScale);
		//FLOAT x, y;
		//for (FLOAT i = XrangeLeft * XplottingScale; i < XrangeRight * XplottingScale; i += 0.01) // x每增长0.01绘制1个点
		//{
		//	x = origin.x + i;
		//	y = origin.y - tan(i/XplottingScale) * YplottingScale;
		//	// 在 (x,y) 处绘制一个点
		//	SetPixel(hdc, x, y, RGB(250, 0, 0));
		//}

		//以上为画函数图象
		

		//Rectangle(hdc, 10, 10, 100, 100);

		SelectObject(hdc, hpenOld);
		//SelectObject(hdc, hbrOld);

		::DeleteObject(hpen1);
		//::DeleteObject(hbr);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
