// LegerityPlotting.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "LegerityPlotting.h"
#include "Resource.h"


#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

HWND console;

BOOL XaxisOn; //�Ƿ���ʾX��
BOOL YaxisOn; //�Ƿ���ʾY��
BOOL gridOn; //�Ƿ���ʾ������
BOOL tickMarksOn; //�Ƿ���ʾ�̶���
BOOL numbersOn; //�Ƿ���ʾ�������ϵ�����

COLORREF backgroundColor; //������ɫ
COLORREF lineColor[7]; //ͼ��������ɫ
COLORREF gridColor; //������ɫ
COLORREF axisColor; //��������ɫ
//COLORREF tickColor; //�̶�����ɫ
COLORREF numberColor; //������ɫ
INT XtickDistance; //X�̶ȸ������̶Ȼ�һ���̶���
INT YtickDistance; //Y�̶ȸ������̶Ȼ�һ���̶���
INT XlabelInterval; //X��������̶��߱�����
INT YlabelInterval; //Y��������̶��߱�����
FLOAT XrangeLeft; //X��Χ
FLOAT XrangeRight; //X�ҷ�Χ
FLOAT YrangeTop; //Y�Ϸ�Χ
FLOAT YrangeBottom; //Y�·�Χ
FLOAT XplottingScale; //X�����ߣ�ʵ��/�߼���
FLOAT YplottingScale; //Y�����ߣ�ʵ��/�߼���
FLOAT tickMarkLength; //�̶��߳���
FLOAT gridSpacing; //���Ӽ��

HWND button_bg, button_gr, button_axis, button_number, 
	button_gr_on, button_axis_on, button_tick_on, button_number_on,
	edit_xtick_distance, edit_ytick_distance, edit_xlabel_interval, edit_ylabel_interval,
	edit_xrange_left, edit_xrange_right, edit_yrange_top, edit_yrange_bottom;


// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Console(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: �ڴ˷��ô��롣

	// ��ʼ��ȫ���ַ���
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LEGERITYPLOTTING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	XaxisOn = TRUE; //�Ƿ���ʾX��
	YaxisOn = TRUE; //�Ƿ���ʾY��
	gridOn = TRUE; //�Ƿ���ʾ������
	tickMarksOn = TRUE; //�Ƿ���ʾ�̶���
	numbersOn = TRUE; //�Ƿ���ʾ�������ϵ�����

	backgroundColor = RGB(255, 255, 255); //������ɫ
	//������ɫ
	lineColor[0] = 0xff0000;
	lineColor[1] = 0xff9900;
	lineColor[2] = 0xffff00;
	lineColor[3] = 0x00ff00;
	lineColor[4] = 0x4a86e8;
	lineColor[5] = 0x0000ff;
	lineColor[6] = 0x9900ff;
	gridColor = RGB(90, 90, 90);
	axisColor = RGB(0, 0, 0);
	numberColor = RGB(0, 0, 0);

	XtickDistance = 1; //X�̶ȸ������̶Ȼ�һ���̶���
	YtickDistance = 1; //Y�̶ȸ������̶Ȼ�һ���̶���
	XlabelInterval = 2; //X��������̶��߱�����
	YlabelInterval = 2; //Y��������̶��߱�����
	XplottingScale = 30; //X�����ߣ�ʵ��/�߼���
	YplottingScale = 30; //Y�����ߣ�ʵ��/�߼���
	tickMarkLength = 20; //�̶��߳���
	gridSpacing = 2; //���Ӽ��

	//X��Y�ķ�Χ 0��ʾδ���� ת�����ݴ��ڴ�С����
	XrangeLeft = 0; //X��Χ
	XrangeRight = 0; //X�ҷ�Χ
	YrangeTop = 0; //Y�Ϸ�Χ
	YrangeBottom = 0; //Y�·�Χ


	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LEGERITYPLOTTING));

	MSG msg;

	// ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		/*RECT plotAreaRect;
		GetClientRect(hWnd, &plotAreaRect);
		HWND label1, label2, label3, label4;
		INT controlAreaLeft = plotAreaRect.right - rightlength;
		label1 = CreateWindow(L"STATIC", L"��ʾ�趨", WS_CHILD | WS_VISIBLE | WS_BORDER,
			controlAreaLeft, 0, rightlength, 20, hWnd, (HMENU)LABEL1, hInst, NULL);
		
		button_bg = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE,
			controlAreaLeft, 20, 20, 20, hWnd, (HMENU)BUTTON_BG, hInst, NULL);
		HWND label_bg = CreateWindow(L"STATIC", L"������ɫ", WS_CHILD | WS_VISIBLE,
			controlAreaLeft + 50, 20, rightlength - 50, 20, hWnd, (HMENU)LABEL_BG, hInst, NULL);
		
		button_gr = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE,
			controlAreaLeft, 40, 20, 20, hWnd, (HMENU)BUTTON_GR, hInst, NULL);
		button_gr_on = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			controlAreaLeft + 30, 40, 20, 20, hWnd, (HMENU)BUTTON_GR_ON, hInst, NULL);
		CheckDlgButton(hWnd, BUTTON_GR_ON, BST_CHECKED);
		HWND label_gr = CreateWindow(L"STATIC", L"����", WS_CHILD | WS_VISIBLE,
			controlAreaLeft + 50, 40, rightlength - 50, 20, hWnd, (HMENU)LABEL_GR, hInst, NULL);
		
		button_axis = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE,
			controlAreaLeft, 60, 20, 20, hWnd, (HMENU)BUTTON_AXIS, hInst, NULL);
		button_axis_on = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			controlAreaLeft + 30, 60, 20, 20, hWnd, (HMENU)BUTTON_AXIS_ON, hInst, NULL);
		CheckDlgButton(hWnd, BUTTON_AXIS_ON, BST_CHECKED);
		HWND label_axis = CreateWindow(L"STATIC", L"����", WS_CHILD | WS_VISIBLE,
			controlAreaLeft + 50, 60, rightlength - 50, 20, hWnd, (HMENU)LABEL_AXIS, hInst, NULL);

		button_tick_on = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			controlAreaLeft + 30, 80, 20, 20, hWnd, (HMENU)BUTTON_TICK_ON, hInst, NULL);
		CheckDlgButton(hWnd, BUTTON_TICK_ON, BST_CHECKED);
		HWND label_tick = CreateWindow(L"STATIC", L"�̶�", WS_CHILD | WS_VISIBLE,
			controlAreaLeft + 50, 80, rightlength - 50, 20, hWnd, (HMENU)LABEL_TICK, hInst, NULL);

		button_number = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE,
			controlAreaLeft, 100, 20, 20, hWnd, (HMENU)BUTTON_NUMBER, hInst, NULL);
		button_number_on = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
			controlAreaLeft + 30, 100, 20, 20, hWnd, (HMENU)BUTTON_NUMBER_ON, hInst, NULL);
		CheckDlgButton(hWnd, BUTTON_NUMBER_ON, BST_CHECKED);
		HWND label_number = CreateWindow(L"STATIC", L"�̶ȱ��", WS_CHILD | WS_VISIBLE,
			controlAreaLeft + 50, 100, rightlength - 50, 20, hWnd, (HMENU)LABEL_NUMBER, hInst, NULL);

		label2 = CreateWindow(L"STATIC", L"������", WS_CHILD | WS_VISIBLE | WS_BORDER,
			controlAreaLeft, 120, rightlength, 20, hWnd, (HMENU)LABEL2, hInst, NULL);

		HWND label_x = CreateWindow(L"STATIC", L"X Tick Distance:", WS_CHILD | WS_VISIBLE,
			controlAreaLeft, 140, 110, 20, hWnd, (HMENU)GENERAL_LABEL, hInst, NULL);
		edit_xtick_distance = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER,
			controlAreaLeft + 110, 140, 30, 20, hWnd, (HMENU)EDIT_XTICK_DISTANCE, hInst, NULL);
		HWND label_y = CreateWindow(L"STATIC", L"Y Tick Distance:", WS_CHILD | WS_VISIBLE,
			controlAreaLeft + 140, 140, 110, 20, hWnd, (HMENU)GENERAL_LABEL, hInst, NULL);
		edit_ytick_distance = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER,
			controlAreaLeft + 250, 140, 30, 20, hWnd, (HMENU)EDIT_YTICK_DISTANCE, hInst, NULL);

		HWND label_xl = CreateWindow(L"STATIC", L"X Label Interval:", WS_CHILD | WS_VISIBLE,
			controlAreaLeft, 160, 110, 20, hWnd, (HMENU)GENERAL_LABEL, hInst, NULL);
		edit_xtick_distance = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER,
			controlAreaLeft + 110, 140, 30, 20, hWnd, (HMENU)EDIT_XTICK_DISTANCE, hInst, NULL);
		HWND label_yl = CreateWindow(L"STATIC", L"Y Label Interval:", WS_CHILD | WS_VISIBLE,
			controlAreaLeft + 160, 140, 110, 20, hWnd, (HMENU)GENERAL_LABEL, hInst, NULL);
		edit_ytick_distance = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER,
			controlAreaLeft + 250, 140, 30, 20, hWnd, (HMENU)EDIT_YTICK_DISTANCE, hInst, NULL);*/

		/*HDC hdc_bg = GetDC(button_bg);
		HBRUSH hBrush_bg = CreateSolidBrush(RGB(0,0,0));
		SelectObject(hdc_bg, hBrush_bg);
		RECT rect_bg;
		GetClientRect(button_bg, &rect_bg);
		FillRect(hdc_bg, &rect_bg, hBrush_bg);*/
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_CONSOLE:
		{
			console = CreateDialog(hInst, MAKEINTATOM(IDD_CONSOLE), hWnd, Console);
			ShowWindow(console, SW_SHOW);
		}
		break;
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
		//{//δ����X��Y�ĵķ�Χ����Ĭ��
		//	XrangeRight = plottingAreaLength / XplottingScale / 2;
		//	XrangeLeft = -XrangeRight;
		//	YrangeBottom = plottingAreaWidth / YplottingScale / 2;
		//	YrangeTop = -YrangeBottom;
		//}


		////ȷ������ԭ��
		////origin.x = plottingAreaRect.right - XrangeRight * XplottingScale;
		////origin.y = plottingAreaRect.bottom - YrangeBottom * YplottingScale;
		//origin.x = plottingAreaLength / 2;
		//origin.y = plottingAreaWidth / 2;

		//// ��ʼ��ͼ
		////SetBkColor(hdc, backgroundColor);


		//if (gridOn == TRUE)
		//{
		//	HPEN hpen2 = ::CreatePen(PS_DASH, 1, RGB(90, 90, 90));
		//	HPEN hpen3 = ::CreatePen(PS_SOLID, 2, RGB(90, 90, 90));
		//	hpenOld = (HPEN)::SelectObject(hMen, hpen2);
		//	INT j = 0;
		//	//X��
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
		//	//Y��
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
		//	//X��̶���
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
		//	//Y��̶���
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
		//	//X������
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
		//	//Y������
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

		////������ͼ��
		//HPEN hpen4 = ::CreatePen(PS_DASH, 2, RGB(250, 0, 0));
		//hpenOld = (HPEN)::SelectObject(hMen, hpen4);
		//FLOAT x, y;
		//for (FLOAT i = XrangeLeft * XplottingScale; i < XrangeRight * XplottingScale; i += 0.01) // xÿ����0.01����1����
		//{
		//	x = origin.x + i;
		//	y = origin.y - tan(i / XplottingScale) * YplottingScale;
		//	// �� (x,y) ������һ����
		//	SetPixel(hMen, x, y, RGB(250, 0, 0));
		//}

		//SelectObject(hMen, hpenOld);
		////SelectObject(hdc, hbrOld);

		//::DeleteObject(hpen1);
		////::DeleteObject(hbr);

		//PAINTSTRUCT ps;
		//HDC hdc2 = BeginPaint(hWnd, &ps);

		//// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
		//BitBlt(hdc, 0, 0, plottingAreaLength, plottingAreaWidth, hMen, 0, 0, SRCCOPY);

		//EndPaint(hWnd, &ps);
		//=========================================================================
		PAINTSTRUCT ps;
		RECT plottingAreaRect;
		POINT origin;
		HDC hdc = BeginPaint(hWnd, &ps);
		INT plottingAreaLength;
		INT plottingAreaWidth;
		HPEN hpenOld;
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...

		GetClientRect(hWnd, &plottingAreaRect);
		plottingAreaLength = plottingAreaRect.right - plottingAreaRect.left;
		plottingAreaWidth = plottingAreaRect.bottom - plottingAreaRect.top;
		

		/*AllocConsole();
		freopen("CONOUT$", "w", stdout);
		printf("i��ֵΪ%d �� %d\n", plottingAreaLength,plottingAreaWidth);
		FreeConsole();*/

		if (XrangeLeft == 0 && XrangeRight == 0
			&& YrangeTop == 0 && YrangeBottom == 0)
		{//δ����X��Y�ĵķ�Χ����Ĭ��
			XrangeRight = plottingAreaLength / XplottingScale / 2;
			XrangeLeft = -XrangeRight;
			YrangeTop = plottingAreaWidth / YplottingScale / 2;
			YrangeBottom = -YrangeTop;
		}

		XplottingScale = plottingAreaLength / (XrangeRight - XrangeLeft);
		YplottingScale = plottingAreaWidth / (YrangeTop - YrangeBottom);

		//ȷ������ԭ��Xֵ
		if (XrangeLeft < 0 && XrangeRight < 0)
		{
			origin.x = plottingAreaRect.right - XrangeRight * XplottingScale;
		}
		else if (XrangeLeft > 0 && XrangeRight > 0)
		{
			origin.x = plottingAreaRect.left - XrangeLeft * XplottingScale;
		}
		else
		{
			origin.x = plottingAreaRect.right * ((-XrangeLeft) / (XrangeRight - XrangeLeft));
		}
		//ȷ������ԭ��Yֵ
		if (YrangeTop < 0 && YrangeBottom < 0)
		{
			origin.y = plottingAreaRect.top + YrangeTop * YplottingScale;
		}
		else if (YrangeTop > 0 && YrangeBottom > 0)
		{
			origin.y = plottingAreaRect.bottom + YrangeBottom * YplottingScale;
		}
		else
		{
			origin.y = plottingAreaRect.bottom * (YrangeTop / (YrangeTop - YrangeBottom));
		}

		// ��ʼ��ͼ
		//SetBkColor(hdc, backgroundColor);

		if (gridOn == TRUE)
		{
			HPEN hpen2 = ::CreatePen(PS_DASH, 1, gridColor);
			HPEN hpen3 = ::CreatePen(PS_SOLID, 2, gridColor);
			hpenOld = (HPEN)::SelectObject(hdc, hpen2);
			INT j = 0;
			//X��
			for (FLOAT i = origin.x; i > plottingAreaRect.left;
			i -= XtickDistance * XplottingScale)
			{
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
				j++;
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
			//Y��
			for (FLOAT i = origin.y; i > plottingAreaRect.top;
			i -= YtickDistance * YplottingScale)
			{
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
				j++;
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
			DeleteObject(hpen2);
			DeleteObject(hpen3);
		}

		if (XaxisOn == TRUE && YaxisOn == TRUE)
		{
			HPEN hpen1 = ::CreatePen(PS_SOLID, 3, axisColor);
			hpenOld = (HPEN)::SelectObject(hdc, hpen1);
			MoveToEx(hdc, 0, origin.y, NULL);
			LineTo(hdc, plottingAreaRect.right, origin.y);
			MoveToEx(hdc, origin.x, 0, NULL);
			LineTo(hdc, origin.x, plottingAreaRect.bottom);
			DeleteObject(hpen1);
		}

		if (tickMarksOn == TRUE)
		{
			HPEN hpen1 = ::CreatePen(PS_SOLID, 3, axisColor);
			hpenOld = (HPEN)::SelectObject(hdc, hpen1);
			INT j = 0;
			//X��̶���
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
			//Y��̶���
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
			DeleteObject(hpen1);
		}
		
		if (numbersOn == TRUE)
		{
			HPEN hpen1 = ::CreatePen(PS_SOLID, 3, numberColor);
			hpenOld = (HPEN)::SelectObject(hdc, hpen1);
			SetTextColor(hdc, numberColor);
			TCHAR buf[20];
			INT j = 0;
			//X������
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
			//Y������
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
			DeleteObject(hpen1);
		}

		//������ͼ��
		HPEN hpen4 = ::CreatePen(PS_DASH, 2, RGB(250, 0, 0));
		hpenOld = (HPEN)::SelectObject(hdc, hpen4);
		plot(hdc, "x^2-2", origin, 
			XrangeLeft, XrangeRight, XplottingScale, YplottingScale);
 		
		SelectObject(hdc, hpenOld);

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

// ��ͼѡ������
INT_PTR CALLBACK Console(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
		else if (LOWORD(wParam) == 1/*IDC_BUTTON1*/)
		{
			
		}
		//else if (LOWORD(wParam) == IDC_BUTTON1)
		//{

		//	TCHAR buf[255];
		//	buf[0] = '\0';

		//	OPENFILENAME ofn;

		//	ZeroMemory(&ofn, sizeof(ofn));

		//	ofn.lStructSize = sizeof(ofn);
		//	ofn.hwndOwner = hDlg;
		//	ofn.hInstance = hInst;
		//	ofn.lpstrFile = buf;
		//	ofn.nMaxFile = 254;
		//	ofn.lpstrFilter = L"ALL";
		//	ofn.nFilterIndex = 1;

		//	GetSaveFileName(&ofn);

		//	MessageBox(hDlg, buf, L"Text", 0);

		//	::SetDlgItemInt(hDlg, IDC_EDIT3,
		//	::GetDlgItemInt(hDlg, IDC_EDIT1, NULL, true) + ::GetDlgItemInt(hDlg, IDC_EDIT2, NULL, true),
		//	NULL);
		//}
		break;
	}
	return (INT_PTR)FALSE;
}

// �����ڡ������Ϣ�������
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
