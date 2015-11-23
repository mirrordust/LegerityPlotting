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
HWND hwndm;
RECT plottingAreaRect;
POINT origin;


BOOL XaxisOn; //�Ƿ���ʾX��
BOOL YaxisOn; //�Ƿ���ʾY��
BOOL gridOn; //�Ƿ���ʾ������
BOOL tickMarksOn; //�Ƿ���ʾ�̶���
BOOL numbersOn; //�Ƿ���ʾ�������ϵ�����

strfunc funcs; //���溯��
strdata impdatas; //����csv����
COLORREF backgroundColor; //������ɫ
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
	funcs.lineColor[0] = 0xff0000;
	funcs.lineColor[1] = 0xff9900;
	funcs.lineColor[2] = 0xffff00;
	funcs.lineColor[3] = 0x00ff00;
	funcs.lineColor[4] = 0x4a86e8;
	funcs.lineColor[5] = 0x0000ff;
	funcs.lineColor[6] = 0x9900ff;
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
		hwndm = hWnd;
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
		case ID_IMPORT:
		{
			wstring filepathname;
			BOOL open = OpenFile(hWnd, filepathname);
			if (open)
			{
				impdatas.number = 0;

				ifstream in;
				in.open(filepathname, ios_base::in);
				std::vector<std::string> result;
				std::string line;
				std::string cell;
				int point_count = 0;
				while (std::getline(in, cell, '\n'))
				{
					point_count++;
					result.push_back(cell);
				}
				POINT* new_points = (POINT*)malloc(sizeof(POINT) * point_count);
				for (int j = 0; j < point_count; j++)
				{
					string s = result.at(j);
					int pos = s.find(',', 0);
					(new_points + j)->x = origin.x + atol(s.substr(0, pos).c_str()) * XplottingScale;
					(new_points + j)->y = origin.y - atol(s.substr(pos + 1, s.length() - pos).c_str()) * YplottingScale;
				}
				COLORREF cr = RGB(0, 0, 0);
				COLORREF cr_new = cscolor(hWnd, cr);
				int index = 0;
				//number
				impdatas.number += 1;
				//linecolor
				COLORREF* crp = new COLORREF[index + 1];
				/*int i = 0;
				for (i = 0; i < index; i++)
				{
					*(crp + i) = *(impdatas.lineColor + i);
				}*/
				*crp = cr_new;
				impdatas.lineColor = crp;
				//pc
				INT* pcp = new INT[index + 1];
				/*for (i = 0; i < index; i++)
				{
					*(pcp + i) = *(impdatas.pc + i);
				}*/
				*pcp = point_count;
				impdatas.pc = pcp;
				//points
				POINT** pnp = (POINT**)malloc(sizeof(POINT*) * (index + 1));
				/*for (i = 0; i < index; i++)
				{
					*(pnp + i) = *(impdatas.points + i);
				}*/
				*pnp = new_points;
				impdatas.points = pnp;
				InvalidateRect(hwndm, NULL, TRUE);
			}
		}
			break;
		case ID_ADD_IMPORT:
		{
			wstring filepathname;
			BOOL open = OpenFile(hWnd, filepathname);
			if (open)
			{
				ifstream in;
				in.open(filepathname, ios_base::in);
				std::vector<std::string> result;
				std::string line;
				std::string cell;
				int point_count = 0;
				while (std::getline(in, cell, '\n'))
				{
					point_count++;
					result.push_back(cell);
				}
				POINT* new_points = (POINT*)malloc(sizeof(POINT) * point_count);
				for (int j = 0; j < point_count; j++)
				{
					string s = result.at(j);
					int pos = s.find(',', 0);
					(new_points + j)->x = origin.x + atol(s.substr(0, pos).c_str()) * XplottingScale;
					(new_points + j)->y = origin.y - atol(s.substr(pos + 1, s.length() - pos).c_str()) * YplottingScale;
				}
				COLORREF cr = RGB(0, 0, 0);
				COLORREF cr_new = cscolor(hWnd, cr);
				int index = impdatas.number;
				//number
				impdatas.number += 1;
				//linecolor
				COLORREF* crp = new COLORREF[index + 1];
				int i = 0;
				for (i = 0; i < index; i++)
				{
					*(crp + i) = *(impdatas.lineColor + i);
				}
				*(crp + i) = cr_new;
				impdatas.lineColor = crp;
				//pc
				INT* pcp = new INT[index + 1];
				for (i = 0; i < index; i++)
				{
					*(pcp + i) = *(impdatas.pc + i);
				}
				*(pcp + i) = point_count;
				impdatas.pc = pcp;
				//points
				POINT** pnp = (POINT**)malloc(sizeof(POINT*) * (index + 1));
				for (i = 0; i < index; i++)
				{
					*(pnp + i) = *(impdatas.points + i);
				}
				*(pnp + i) = new_points;
				impdatas.points = pnp;
				InvalidateRect(hwndm, NULL, TRUE);
			}
		}
			break;
		case ID_CLEAR_IMPORT:
		{
			impdatas.number = 0;
			InvalidateRect(hwndm, NULL, TRUE);
		}
			break;
		case IDM_CONSOLE:
		{
			console = CreateDialog(hInst, MAKEINTATOM(IDD_CONSOLE), hWnd, Console);
			ShowWindow(console, SW_SHOW);
			if (gridOn == TRUE)
				CheckDlgButton(console, IDC_GR_ON, BST_CHECKED);
			if (XaxisOn == TRUE && YaxisOn == TRUE)
				CheckDlgButton(console, IDC_AXIS_ON, BST_CHECKED);
			if (tickMarksOn == TRUE)
				CheckDlgButton(console, IDC_TICK_ON, BST_CHECKED);
			if (numbersOn == TRUE)
				CheckDlgButton(console, IDC_NUMBER_ON, BST_CHECKED);

			TCHAR buf[100];
			swprintf(buf, L"%f", XrangeLeft);
			SetDlgItemText(console, IDC_EDIT_X_RANGE_LEFT, buf);
			swprintf(buf, L"%f", XrangeRight);
			SetDlgItemText(console, IDC_EDIT_X_RANGE_RIGHT, buf);
			swprintf(buf, L"%f", YrangeTop);
			SetDlgItemText(console, IDC_EDIT_Y_RANGE_TOP, buf);
			swprintf(buf, L"%f", YrangeBottom);
			SetDlgItemText(console, IDC_EDIT_Y_RANGE_BOTTOM, buf);
			SetDlgItemInt(console, IDC_EDIT_X_TICK_DISTANCE, XtickDistance, TRUE);
			SetDlgItemInt(console, IDC_EDIT_Y_TICK_DISTANCE, YtickDistance, TRUE);
			SetDlgItemInt(console, IDC_EDIT_X_LABEL_INTERVAL, XlabelInterval, TRUE);
			SetDlgItemInt(console, IDC_EDIT_Y_LABEL_INTERVAL, YlabelInterval, TRUE);

		}
			break;
		case IDM_SAVE:
		{
			HDC hdc = GetDC(hWnd);
			HDC hMemDc = CreateCompatibleDC(hdc);
			LONG width = plottingAreaRect.right - plottingAreaRect.left;
			LONG height = plottingAreaRect.bottom - plottingAreaRect.top;
			HBITMAP hbmp = CreateCompatibleBitmap(hdc, width, height);
			SelectObject(hMemDc, hbmp);
			BitBlt(hMemDc, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
			int nBits = GetDeviceCaps(hdc, BITSPIXEL);
			DWORD dwLineBits = 0;
			switch (nBits)
			{
			case 16:
				dwLineBits = width * 2;
				break;
			case 24:
				dwLineBits = (width + 1) * 3 - ((width + 1) * 3) % 4;
				break;
			case 32:
				dwLineBits = width * 4;
				break;
			default:
				//MessageBox(NULL, _T("��֧�ָø�ʽ��"), _T("��֧�֣�"), MB_OK);
				break;
			}
			DWORD dwBitsCount = dwLineBits * height;
			DWORD dwFileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwBitsCount;
			BYTE * pMem = (BYTE *)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, dwFileSize);
			if (NULL == pMem)
			{
				MessageBox(hWnd, _T("����ʧ�ܣ�"), _T("����"), MB_OK);
			}
			BITMAPFILEHEADER* pBMPHeader = (BITMAPFILEHEADER*)pMem;
			BITMAPINFOHEADER* pInfoHeader = (BITMAPINFOHEADER*)((BYTE *)pBMPHeader + sizeof(BITMAPFILEHEADER));
			BYTE* pBits = (BYTE*)pInfoHeader + sizeof(BITMAPINFOHEADER);
			//��ʼ��fileheader��infoheader
			pBMPHeader->bfType = ('M' << 8) | 'B';
			pBMPHeader->bfSize = dwFileSize;
			pBMPHeader->bfReserved1 = 0;
			pBMPHeader->bfReserved2 = 0;
			pBMPHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

			pInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
			pInfoHeader->biWidth = width;
			pInfoHeader->biHeight = height;
			pInfoHeader->biPlanes = GetDeviceCaps(hdc, PLANES);  //��ɫ����
			pInfoHeader->biBitCount = nBits;
			pInfoHeader->biCompression = BI_RGB;
			pInfoHeader->biSizeImage = dwBitsCount;
			pInfoHeader->biXPelsPerMeter = GetDeviceCaps(hdc, LOGPIXELSX);
			pInfoHeader->biYPelsPerMeter = GetDeviceCaps(hdc, LOGPIXELSY);
			pInfoHeader->biClrUsed = 0;
			pInfoHeader->biClrImportant = 0;
			//ͼ������
			int nLines = GetDIBits(hdc, hbmp, 0, height, pBits,
				(BITMAPINFO *)pInfoHeader, DIB_RGB_COLORS);
			if (0 == nLines)
			{
				//DWORD errno = GetLastError();
				MessageBox(hWnd, _T("����ʧ�ܣ�"), _T("����"), MB_OK);
			}
			ReleaseDC(hWnd, hdc);
			DeleteDC(hMemDc);
			hMemDc = NULL;
			DeleteObject(hbmp);

			wstring filepathname;
			//SelectDir(hWnd, filepathname);
			BOOL save = SaveAsBmp(hWnd, filepathname);

			if (save)
			{
				//�����ļ�
				HANDLE hFile = CreateFile(filepathname.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				//CreateFile(L"TESTimage.bmp", GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
				/*if (NULL == hFile || INVALID_HANDLE_VALUE == hFile)
				{
					MessageBox(hWnd, _T("�ļ��Ѿ����ڣ�"), _T("����"), MB_OK);
				}*/
				//д��
				DWORD dwWriteCount = 0;
				if (!WriteFile(hFile, (void *)pBMPHeader, dwFileSize, &dwWriteCount, NULL))
				{
					MessageBox(NULL, _T("д���ļ�ʧ�ܣ�"), _T("����"), MB_OK | MB_ICONEXCLAMATION);
					CloseHandle(hFile);
				}
				//�ر��ļ�
				CloseHandle(hFile);
				GlobalFree((void *)pBMPHeader);
			}
			//=============================
			//=============================

			/*HDC hdc = GetDC(hWnd);
			BYTE* buf = new BYTE[plottingAreaRect.right * plottingAreaRect.bottom * 3];
			memset(buf, 0, plottingAreaRect.right * plottingAreaRect.bottom);
			for (int i = 0; i < plottingAreaRect.bottom; i++)
			{
				for (int j = 0; j < plottingAreaRect.right; j++)
				{
					COLORREF cr = GetPixel(hdc, j, i);
					LONG t = i*plottingAreaRect.right + j;
					buf[t * 3] = GetRValue(cr);
					buf[t * 3 + 1] = GetGValue(cr);
					buf[t * 3 + 2] = GetBValue(cr);
				}
			}
			long newsize;
			BYTE* bufmb = ConvertRGBToBMPBuffer(buf, plottingAreaRect.right,
				plottingAreaRect.bottom, &newsize);
			SaveBMP(bufmb, plottingAreaRect.right, plottingAreaRect.bottom, newsize, L"TESTimage.bmp");
			delete(bufmb);
			MessageBox(hWnd, L"����ɹ�", L"��ʾ", 0);
			ReleaseDC(hWnd, hdc);*/
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
	case WM_MOUSEWHEEL:
	{
		SHORT zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		FLOAT change;
		if (zDelta > 0)
		{
			change = zDelta / 120;
			XplottingScale = XplottingScale + change * 5;
			YplottingScale = YplottingScale + change * 5;
			printf("%f,%f\n",XplottingScale,YplottingScale);
		}
		else
		{
			change = zDelta / 120;
			if (XplottingScale + change * 5 > 0)
			{
				XplottingScale = XplottingScale + change * 5;
			}
			if (YplottingScale + change * 5 > 0)
			{
				YplottingScale = YplottingScale + change * 5;
			}
			printf("%f,%f\n", XplottingScale, YplottingScale);
		}
		FLOAT Xmiddle = (XrangeRight + XrangeLeft) / 2;
		FLOAT Ymiddle = (YrangeTop + YrangeBottom) / 2;
		XrangeRight = Xmiddle + plottingAreaRect.right / 2 / XplottingScale;
		XrangeLeft = Xmiddle - plottingAreaRect.right / 2 / XplottingScale;
		YrangeTop = Ymiddle + plottingAreaRect.bottom / 2 / YplottingScale;
		YrangeBottom = Ymiddle - plottingAreaRect.bottom / 2 / YplottingScale;

		InvalidateRect(hwndm, NULL, TRUE);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		//RECT plottingAreaRect;
		HDC hdc = BeginPaint(hWnd, &ps);
		INT plottingAreaLength;
		INT plottingAreaWidth;
		HPEN hpenOld;
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...

		GetClientRect(hWnd, &plottingAreaRect);
		plottingAreaLength = plottingAreaRect.right - plottingAreaRect.left;
		plottingAreaWidth = plottingAreaRect.bottom - plottingAreaRect.top;

		if (XrangeLeft == 0 && XrangeRight == 0
			&& YrangeTop == 0 && YrangeBottom == 0)
		{//δ����X��Y�ĵķ�Χ����Ĭ��
			XrangeRight = plottingAreaLength / XplottingScale / 2;
			XrangeLeft = -XrangeRight;
			YrangeTop = plottingAreaWidth / YplottingScale / 2;
			YrangeBottom = -YrangeTop;
		}

		//XplottingScale = plottingAreaLength / (XrangeRight - XrangeLeft);
		//YplottingScale = plottingAreaWidth / (YrangeTop - YrangeBottom);

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

		HBRUSH hBrush_bg = CreateSolidBrush(backgroundColor);
		SelectObject(hdc, hBrush_bg);
		RECT rect_bg;
		GetClientRect(hWnd, &rect_bg);
		FillRect(hdc, &rect_bg, hBrush_bg);

		if (gridOn == TRUE)
		{
			HPEN hpen2 = ::CreatePen(PS_DASH, 1, gridColor);
			HPEN hpen3 = ::CreatePen(PS_DASH, 1, gridColor);
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
			INT numval = 0;
			//X������
			for (FLOAT i = origin.x - XtickDistance * XplottingScale; i > plottingAreaRect.left;
			i -= XtickDistance * XplottingScale)
			{
				j--;
				numval -= XtickDistance;
				if (j % XlabelInterval == 0)
				{
					::wsprintf(buf, L"%d", numval);
					SetTextAlign(hdc, TA_CENTER);
					TextOut(hdc, i - wcslen(buf) / 2, origin.y + tickMarkLength / 2, buf, ::wcslen(buf));
				}
			}
			j = 0;
			numval = 0;
			for (FLOAT i = origin.x + XtickDistance * XplottingScale; i < plottingAreaRect.right;
			i += XtickDistance * XplottingScale)
			{
				j++;
				numval += XtickDistance;
				if (j % XlabelInterval == 0)
				{
					::wsprintf(buf, L"%d", numval);
					TextOut(hdc, i - wcslen(buf) / 2, origin.y + tickMarkLength / 2, buf, ::wcslen(buf));
				}
			}
			j = 0;
			numval = 0;
			SetTextAlign(hdc, TA_BASELINE + TA_RIGHT);
			//Y������
			for (FLOAT i = origin.y - YtickDistance * YplottingScale; i > plottingAreaRect.top;
			i -= YtickDistance * YplottingScale)
			{
				j++;
				numval += YtickDistance;
				if (j % YlabelInterval == 0)
				{
					::wsprintf(buf, L"%d", numval);
					TextOut(hdc, origin.x - tickMarkLength / 2 - wcslen(buf), i, buf, ::wcslen(buf));
				}
			}
			j = 0;
			numval = 0;
			for (FLOAT i = origin.y + YtickDistance * YplottingScale; i < plottingAreaRect.bottom;
			i += YtickDistance * YplottingScale)
			{
				j--;
				numval -= YtickDistance;
				if (j % YlabelInterval == 0)
				{
					::wsprintf(buf, L"%d", numval);
					TextOut(hdc, origin.x - tickMarkLength / 2 - wcslen(buf), i, buf, ::wcslen(buf));
				}
			}
			DeleteObject(hpen1);
		}

		//������ͼ��
		int function_num = funcs.number;
		int c = 0;
		while (c < function_num)
		{
			HPEN hPen = CreatePen(PS_DASH, 2, funcs.lineColor[c]);
			hpenOld = (HPEN)SelectObject(hdc, hPen);
			plot(hdc, funcs.functions[c], origin, XrangeLeft, XrangeRight, XplottingScale, YplottingScale);
			DeleteObject(hPen);
			c++;
		}
		//����������
		int impnumber = impdatas.number;
		for (int i = 0; i < impnumber; i++)
		{
			HPEN hPen = CreatePen(PS_DASH, 2, impdatas.lineColor[i]);
			hpenOld = (HPEN)SelectObject(hdc, hPen);
			Polyline(hdc, impdatas.points[i], impdatas.pc[i]);
		}
		/*HPEN hpen4 = ::CreatePen(PS_DASH, 2, RGB(250, 0, 0));
		hpenOld = (HPEN)::SelectObject(hdc, hpen4);
		plot(hdc, "x^2+3.5*x", origin,
			XrangeLeft, XrangeRight, XplottingScale, YplottingScale);

		DeleteObject(hpen4);*/


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
		if (LOWORD(wParam) == IDCANCEL)
		{
			//EndDialog(hDlg, LOWORD(wParam));
			DestroyWindow(console);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_BUTTON_BG)
		{
			HWND hb = GetDlgItem(hDlg, IDC_BUTTON_BG);
			backgroundColor = cscolor(hb, backgroundColor);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_GR)
		{
			HWND hb = GetDlgItem(hDlg, IDC_BUTTON_GR);
			gridColor = cscolor(hb, gridColor);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_AXIS)
		{
			HWND hb = GetDlgItem(hDlg, IDC_BUTTON_AXIS);
			axisColor = cscolor(hb, axisColor);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_NUMBER)
		{
			HWND hb = GetDlgItem(hDlg, IDC_BUTTON_NUMBER);
			numberColor = cscolor(hb, numberColor);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_FUNC1)
		{
			HWND hb = GetDlgItem(hDlg, IDC_BUTTON_FUNC1);
			funcs.lineColor[0] = cscolor(hb, funcs.lineColor[0]);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_FUNC2)
		{
			HWND hb = GetDlgItem(hDlg, IDC_BUTTON_FUNC2);
			funcs.lineColor[1] = cscolor(hb, funcs.lineColor[1]);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_FUNC3)
		{
			HWND hb = GetDlgItem(hDlg, IDC_BUTTON_FUNC3);
			funcs.lineColor[2] = cscolor(hb, funcs.lineColor[2]);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_FUNC4)
		{
			HWND hb = GetDlgItem(hDlg, IDC_BUTTON_FUNC4);
			funcs.lineColor[3] = cscolor(hb, funcs.lineColor[3]);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_FUNC5)
		{
			HWND hb = GetDlgItem(hDlg, IDC_BUTTON_FUNC5);
			funcs.lineColor[4] = cscolor(hb, funcs.lineColor[4]);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_FUNC6)
		{
			HWND hb = GetDlgItem(hDlg, IDC_BUTTON_FUNC6);
			funcs.lineColor[5] = cscolor(hb, funcs.lineColor[5]);
		}
		else if (LOWORD(wParam) == IDC_BUTTON_FUNC7)
		{
			HWND hb = GetDlgItem(hDlg, IDC_BUTTON_FUNC7);
			funcs.lineColor[6] = cscolor(hb, funcs.lineColor[6]);
		}
		else if (LOWORD(wParam) == IDPLOT)
		{
			if (IsDlgButtonChecked(hDlg, IDC_GR_ON))
			{
				gridOn = TRUE;
			}
			else {
				gridOn = FALSE;
			}
			if (IsDlgButtonChecked(hDlg, IDC_AXIS_ON))
			{
				XaxisOn = TRUE;
				YaxisOn = TRUE;
			}
			else {
				XaxisOn = FALSE;
				YaxisOn = FALSE;
			}
			if (IsDlgButtonChecked(hDlg, IDC_TICK_ON))
			{
				tickMarksOn = TRUE;
			}
			else {
				tickMarksOn = FALSE;
			}
			if (IsDlgButtonChecked(hDlg, IDC_NUMBER_ON))
			{
				numbersOn = TRUE;
			}
			else
			{
				numbersOn = FALSE;
			}

			TCHAR buf[100];
			GetDlgItemText(hDlg, IDC_EDIT_X_RANGE_LEFT, buf, 100);
			XrangeLeft = _wtof(buf);
			GetDlgItemText(hDlg, IDC_EDIT_X_RANGE_RIGHT, buf, 100);
			XrangeRight = _wtof(buf);
			GetDlgItemText(hDlg, IDC_EDIT_Y_RANGE_TOP, buf, 100);
			YrangeTop = _wtof(buf);
			GetDlgItemText(hDlg, IDC_EDIT_Y_RANGE_BOTTOM, buf, 100);
			YrangeBottom = _wtof(buf);
			XplottingScale = plottingAreaRect.right / (XrangeRight - XrangeLeft);
			YplottingScale = plottingAreaRect.bottom / (YrangeTop - YrangeBottom);
			//XplottingScale = plottingAreaLength / (XrangeRight - XrangeLeft);
			//YplottingScale = plottingAreaWidth / (YrangeTop - YrangeBottom);

			XtickDistance = GetDlgItemInt(hDlg, IDC_EDIT_X_TICK_DISTANCE, NULL, TRUE);
			YtickDistance = GetDlgItemInt(hDlg, IDC_EDIT_Y_TICK_DISTANCE, NULL, TRUE);
			XlabelInterval = GetDlgItemInt(hDlg, IDC_EDIT_X_LABEL_INTERVAL, NULL, TRUE);
			YlabelInterval = GetDlgItemInt(hDlg, IDC_EDIT_Y_LABEL_INTERVAL, NULL, TRUE);

			//f1
			funcs.number = 0;
			TCHAR funbuf[500];
			GetDlgItemText(hDlg, IDC_EDIT_FUNC1, funbuf, 500);
			size_t funbufsize = wcslen(funbuf) + 1;
			size_t convertedChars = 0;
			const size_t newsize1 = funbufsize * 2;
			char* nstr = new char[newsize1];
			wcstombs_s(&convertedChars, nstr, newsize1, funbuf, _TRUNCATE);
			string ns(nstr);
			if (!(ns == ""))
			{
				funcs.functions[0] = ns;
				funcs.number += 1;
			}
			//f2
			GetDlgItemText(hDlg, IDC_EDIT_FUNC2, funbuf, 500);
			funbufsize = wcslen(funbuf) + 1;
			convertedChars = 0;
			const size_t newsize2 = funbufsize * 2;
			nstr = new char[newsize2];
			wcstombs_s(&convertedChars, nstr, newsize2, funbuf, _TRUNCATE);
			ns = string(nstr);
			if (!(ns == ""))
			{
				funcs.functions[1] = ns;
				funcs.number += 1;
			}
			//f3
			GetDlgItemText(hDlg, IDC_EDIT_FUNC3, funbuf, 500);
			funbufsize = wcslen(funbuf) + 1;
			convertedChars = 0;
			const size_t newsize3 = funbufsize * 2;
			nstr = new char[newsize3];
			wcstombs_s(&convertedChars, nstr, newsize3, funbuf, _TRUNCATE);
			ns = string(nstr);
			if (!(ns == ""))
			{
				funcs.functions[2] = ns;
				funcs.number += 1;
			}
			//f4
			GetDlgItemText(hDlg, IDC_EDIT_FUNC4, funbuf, 500);
			funbufsize = wcslen(funbuf) + 1;
			convertedChars = 0;
			const size_t newsize4 = funbufsize * 2;
			nstr = new char[newsize4];
			wcstombs_s(&convertedChars, nstr, newsize4, funbuf, _TRUNCATE);
			ns = string(nstr);
			if (!(ns == ""))
			{
				funcs.functions[3] = ns;
				funcs.number += 1;
			}
			//f5
			GetDlgItemText(hDlg, IDC_EDIT_FUNC5, funbuf, 500);
			funbufsize = wcslen(funbuf) + 1;
			convertedChars = 0;
			const size_t newsize5 = funbufsize * 2;
			nstr = new char[newsize5];
			wcstombs_s(&convertedChars, nstr, newsize5, funbuf, _TRUNCATE);
			ns = string(nstr);
			if (!(ns == ""))
			{
				funcs.functions[4] = ns;
				funcs.number += 1;
			}
			//f6
			GetDlgItemText(hDlg, IDC_EDIT_FUNC6, funbuf, 500);
			funbufsize = wcslen(funbuf) + 1;
			convertedChars = 0;
			const size_t newsize6 = funbufsize * 2;
			nstr = new char[newsize6];
			wcstombs_s(&convertedChars, nstr, newsize6, funbuf, _TRUNCATE);
			ns = string(nstr);
			if (!(ns == ""))
			{
				funcs.functions[5] = ns;
				funcs.number += 1;
			}
			//f7
			GetDlgItemText(hDlg, IDC_EDIT_FUNC7, funbuf, 500);
			funbufsize = wcslen(funbuf) + 1;
			convertedChars = 0;
			const size_t newsize7 = funbufsize * 2;
			nstr = new char[newsize7];
			wcstombs_s(&convertedChars, nstr, newsize7, funbuf, _TRUNCATE);
			ns = string(nstr);
			if (!(ns == ""))
			{
				funcs.functions[6] = ns;
				funcs.number += 1;
			}

			InvalidateRect(hwndm, NULL, TRUE);
			SendMessage(hwndm, WM_PAINT, NULL, NULL);
		}
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


