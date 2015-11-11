// LegerityPlotting.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "LegerityPlotting.h"


#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

BOOL XaxisOn; //�Ƿ���ʾX��
BOOL YaxisOn; //�Ƿ���ʾY��
BOOL gridOn; //�Ƿ���ʾ������
BOOL tickMarksOn; //�Ƿ���ʾ�̶���
BOOL numbersOn; //�Ƿ���ʾ�������ϵ�����

COLORREF backgroundColor; //������ɫ
COLORREF lineColor; //ͼ��������ɫ
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


// �˴���ģ���а����ĺ�����ǰ������: 
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

	backgroundColor = RGB(255, 0, 0); //������ɫ
	lineColor = RGB(0, 0, 0); //ͼ��������ɫ

	XtickDistance = 1; //X�̶ȸ������̶Ȼ�һ���̶���
	YtickDistance = 1; //Y�̶ȸ������̶Ȼ�һ���̶���
	XlabelInterval = 5; //X��������̶��߱�����
	YlabelInterval = 5; //Y��������̶��߱�����
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
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// �����˵�ѡ��: 
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

		////����Ϊ������ͼ��


		////Rectangle(hdc, 10, 10, 100, 100);

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
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
		HPEN hpen1 = ::CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		HPEN hpenOld = (HPEN)::SelectObject(hdc, hpen1);

		//HBRUSH hbr = ::CreateSolidBrush(0xabcdef);
		//HBRUSH hbrOld = (HBRUSH)::SelectObject(hdc, hbr);
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
			YrangeBottom = plottingAreaWidth / YplottingScale / 2;
			YrangeTop = -YrangeBottom;
		}

		/*XrangeRight = 20;
		XrangeLeft = -7;
		YrangeBottom = 13;
		YrangeTop = -16;
		XplottingScale = plottingAreaLength / (XrangeRight - XrangeLeft);
		YplottingScale = plottingAreaWidth / (YrangeBottom - YrangeTop);*/

		//ȷ������ԭ��
		//origin.x = plottingAreaRect.right - XrangeRight * XplottingScale;
		//origin.y = plottingAreaRect.bottom - YrangeBottom * YplottingScale;
		origin.x = plottingAreaLength / 2;
		origin.y = plottingAreaWidth / 2;

		// ��ʼ��ͼ
		//SetBkColor(hdc, backgroundColor);

		
		if (gridOn == TRUE)
		{
			HPEN hpen2 = ::CreatePen(PS_DASH, 1, RGB(90, 90, 90));
			HPEN hpen3 = ::CreatePen(PS_SOLID, 2, RGB(90, 90, 90));
			hpenOld = (HPEN)::SelectObject(hdc, hpen2);
			INT j = 0;
			//X��
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
			//Y��
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
		}
		
		if (numbersOn == TRUE)
		{
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
		}

		//������ͼ��
		HPEN hpen4 = ::CreatePen(PS_DASH, 2, RGB(250, 0, 0));
		hpenOld = (HPEN)::SelectObject(hdc, hpen4);
		plot(hdc, RGB(250,0,0), "3^x", origin, 
			XrangeLeft, XrangeRight, XplottingScale, YplottingScale);
		//FLOAT x, y;
		//for (FLOAT i = XrangeLeft * XplottingScale; i < XrangeRight * XplottingScale; i += 0.01) // xÿ����0.01����1����
		//{
		//	x = origin.x + i;
		//	y = origin.y - tan(i/XplottingScale) * YplottingScale;
		//	// �� (x,y) ������һ����
		//	SetPixel(hdc, x, y, RGB(250, 0, 0));
		//}

		//����Ϊ������ͼ��
		

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
