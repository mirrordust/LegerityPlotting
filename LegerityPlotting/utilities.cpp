#include "stdafx.h"
#include "utilities.h"


using namespace std;

void plot(HDC hdc, string expression, POINT origin,
	float XrangeLeft, float XrangeRight, float XplottingScale, float YplottingScale)
{
	queue<string> rpn = postfix(expression);
	
	INT pn = (XrangeRight - XrangeLeft) * XplottingScale + 2; //point number
	POINT* points = new POINT[pn];
	int pc = 0;

	for (FLOAT i = XrangeLeft * XplottingScale; i < XrangeRight * XplottingScale; i += 1) // x每增长0.01绘制1个点
	{
		LONG x = origin.x + i;

		/*AllocConsole();
		freopen("CONOUT$", "w", stdout);
		printf("%f", i / XplottingScale);
		FreeConsole();*/
		
		calres result = calculate(rpn, i / XplottingScale);
		
		if (result.valid)
		{
			LONG y = origin.y - result.answer * YplottingScale;
			points[pc].x = x;
			points[pc].y = y;
			pc++;
		}

		//y = origin.y - tan(i / XplottingScale) * YplottingScale;
		// 在 (x,y) 处绘制一个点
		//SetPixel(hdc, x, y, RGB(250, 0, 0));
	}
	//PolylineTo画图
	int re = Polyline(hdc, points, pc);
	if (re == 0)
	{
		TCHAR buf[1000];
		::wsprintf(buf, L"=======> %d和%d", points[10].x,points[10].y);
		TextOut(hdc, 300, 300, buf, ::wcslen(buf));
	}

}

calres calculate(queue<string> rpn, float x)
{
	calres result;
	result.valid = true;
	stack<double> st_result;
	int j = rpn.size();
	while (j--)
	{
		string s = rpn.front();
		rpn.pop();
		if (s == "x")
		{
			s = to_string(x);
		}
		rpn.push(s);
	}
	while (!rpn.empty())
	{
		string s = rpn.front();
		rpn.pop();
		if (isdigit(s[0]) || (s[0] == '-' && isdigit(s[1])))
		{
			double d = atof(s.c_str());
			st_result.push(d);
		}
		else
		{
			char op = s[0];
			double right = st_result.top();
			st_result.pop();
			double left = st_result.top();
			st_result.pop();
			switch (op)
			{
			case '+':
			{	double answer = left + right;
			st_result.push(answer);
			break;
			}
			case '-':
			{
				double answer = left - right;
				st_result.push(answer);
				break;
			}
			case '*':
			{
				double answer = left * right;
				st_result.push(answer);
				break;
			}
			case '/':
			{
				if (right == 0)
				{
					//报错
					result.valid = false;
					break;
				}
				double answer = left / right;
				st_result.push(answer);
				break;
			}
			case '%':
			{
				//double answer = left % right;
				//st_result.push(answer);
				break;
			}
			case '^':
			{
				double answer = pow(left, right);
				st_result.push(answer);
				break;
			}
			default:
				break;
			}
		}
	}

	result.answer = st_result.top();
	st_result.pop();
	return result;
}

//将输入的式子转为RPN
queue<string> postfix(string expression)
{
	char ch, ch_pre = '#';
	for (unsigned int i = 0; i < expression.length();)
	{
		ch = expression.at(i++);
		if (isdigit(ch_pre) && ch == 'x')
		{
			char x = '*';
			expression = expression.insert(i - 1, &x, 1);
			ch = expression.at(++i);
			ch_pre = 'x';
		}
		else
		{
			ch_pre = ch;
		}

	}
	//cout << expression << endl;
	map<char, int> isp;
	isp.insert(pair<char, int>('#', 0));
	isp.insert(pair<char, int>('(', 1));
	isp.insert(pair<char, int>('*', 5));
	isp.insert(pair<char, int>('/', 5));
	isp.insert(pair<char, int>('%', 5));
	isp.insert(pair<char, int>('+', 3));
	isp.insert(pair<char, int>('-', 3));
	isp.insert(pair<char, int>(')', 8));
	isp.insert(pair<char, int>('^', 6));
	map<char, int> icp;
	icp.insert(pair<char, int>('#', 0));
	icp.insert(pair<char, int>('(', 8));
	icp.insert(pair<char, int>('*', 4));
	icp.insert(pair<char, int>('/', 4));
	icp.insert(pair<char, int>('%', 4));
	icp.insert(pair<char, int>('+', 2));
	icp.insert(pair<char, int>('-', 2));
	icp.insert(pair<char, int>(')', 1));
	icp.insert(pair<char, int>('^', 7));

	string t_str = expression + '#';
	int index = 0;
	stack<char> symbol;
	queue<string> result;
	string s;
	ch = '#';
	symbol.push(ch);
	ch = t_str.at(index++);
	while (!symbol.empty() || ch != '#')
	{
		switch (judgeX(ch))
		{
		case 0:
		{
			s = string(&ch, 1);
			ch = t_str.at(index++);
			while (isdigit(ch) || ch == '.')
			{
				s += ch;
				ch = t_str.at(index++);
			}
			index -= 1;
			result.push(s);
			ch = t_str.at(index++);
			break;
		}
		case 1:
		{
			s = string(&ch, 1);
			result.push(s);
			ch = t_str.at(index++);
			break;
		}
		case -1:
		{
			char& symbol_top = symbol.top();
			if (isp[symbol_top] < icp[ch])
			{
				symbol.push(ch);
				ch = t_str.at(index++);
			}
			else if (isp[symbol_top] > icp[ch])
			{
				char& op = symbol.top();
				symbol.pop();
				s = string(&op, 1);
				result.push(s);
			}
			else
			{
				char& op = symbol.top();
				symbol.pop();
				if (op == '(')
				{
					ch = t_str.at(index++);
				}
			}
			break;
		}
		default:
			//输入有误，报错
			break;
		}
	}
	return result;
}

//判断下一个读取的字符是数字还是符号还是x
int judgeX(char ch)
{

	if (isdigit(ch))
	{
		return 0;
	}
	else if (ch == 'x')
	{
		return 1;
	}
	else
	{
		switch (ch)
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '^':
		case '#':
		case '(':
		case ')':
			return -1;
		default:
			return -2;
		}
	}
}

COLORREF cscolor(HWND hButton, COLORREF pre)
{
	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
	static COLORREF rgbCurrent = RGB(0, 0, 0);        // initial color selection

	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;
	cc.hwndOwner = hButton;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc) == TRUE)
	{
		//hbrush = CreateSolidBrush(cc.rgbResult);
		rgbCurrent = cc.rgbResult;
		return rgbCurrent;
	}
	else
	{
		return pre;
	}
}

bool SaveBMP(BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile)
{
	// declare bmp structures 
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER info;

	// andinitialize them to zero
	memset(&bmfh, 0, sizeof(BITMAPFILEHEADER));
	memset(&info, 0, sizeof(BITMAPINFOHEADER));

	// fill the fileheader with data
	bmfh.bfType = 0x4d42;       // 0x4d42 = 'BM'
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + paddedsize;
	bmfh.bfOffBits = 0x36;		// number of bytes to start of bitmap bits

								// fill the infoheader

	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = width;
	info.biHeight = height;
	info.biPlanes = 1;			// we only have one bitplane
	info.biBitCount = 24;		// RGB mode is 24 bits
	info.biCompression = BI_RGB;
	info.biSizeImage = 0;		// can be 0 for 24 bit images
	info.biXPelsPerMeter = 0x0ec4;     // paint and PSP use this values
	info.biYPelsPerMeter = 0x0ec4;
	info.biClrUsed = 0;			// we are in RGB mode and have no palette
	info.biClrImportant = 0;    // all colors are important

								// now we open the file to write to
	HANDLE file = CreateFile(bmpfile, GENERIC_WRITE, FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == NULL)
	{
		CloseHandle(file);
		return false;
	}

	// write file header
	unsigned long bwritten;
	if (WriteFile(file, &bmfh, sizeof(BITMAPFILEHEADER), &bwritten, NULL) == false)
	{
		CloseHandle(file);
		return false;
	}
	// write infoheader
	if (WriteFile(file, &info, sizeof(BITMAPINFOHEADER), &bwritten, NULL) == false)
	{
		CloseHandle(file);
		return false;
	}
	// write image data
	if (WriteFile(file, Buffer, paddedsize, &bwritten, NULL) == false)
	{
		CloseHandle(file);
		return false;
	}

	// and close file
	CloseHandle(file);

	return true;
}

BYTE* ConvertRGBToBMPBuffer(BYTE* Buffer, int width, int height, long* newsize)
{

	// first make sure the parameters are valid
	if ((NULL == Buffer) || (width == 0) || (height == 0))
		return NULL;

	// now we have to find with how many bytes
	// we have to pad for the next DWORD boundary	

	int padding = 0;
	int scanlinebytes = width * 3;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

	// we can already store the size of the new padded buffer
	*newsize = height * psw;

	// and create new buffer
	BYTE* newbuf = new BYTE[*newsize];

	// fill the buffer with zero bytes then we dont have to add
	// extra padding zero bytes later on
	memset(newbuf, 0, *newsize);

	// now we loop trough all bytes of the original buffer, 
	// swap the R and B bytes and the scanlines
	long bufpos = 0;
	long newpos = 0;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < 3 * width; x += 3)
		{
			bufpos = y * 3 * width + x;     // position in original buffer
			newpos = (height - y - 1) * psw + x;           // position in padded buffer

			newbuf[newpos] = Buffer[bufpos + 2];       // swap r and b
			newbuf[newpos + 1] = Buffer[bufpos + 1]; // g stays
			newbuf[newpos + 2] = Buffer[bufpos];     // swap b and r
		}

	return newbuf;
}

BOOL SelectDir(HWND hWnd, wstring &strPath)
{
	BROWSEINFO bifolder;
	wchar_t FileName[MAX_PATH];
	ZeroMemory(&bifolder, sizeof(BROWSEINFO));
	bifolder.hwndOwner = hWnd;				// 拥有者句柄
	bifolder.pszDisplayName = FileName;		// 存放目录路径缓冲区
	bifolder.lpszTitle = TEXT("请选择文件夹");	// 标题
	bifolder.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX; // 新的样式,带编辑框

	LPITEMIDLIST idl = SHBrowseForFolder(&bifolder);
	if (idl == NULL)
	{
		return FALSE;
	}

	SHGetPathFromIDList(idl, FileName);
	strPath = FileName;
	wcout.imbue(locale("chs"));
	wcout << L"所选目录: " << strPath << endl;
	return TRUE;
}

BOOL SaveAsBmp(HWND hwnd, wstring &strPath)
{
	wchar_t szPathName[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };//or  {sizeof (OPENFILENAME)}  
	ofn.hwndOwner = hwnd;			// 拥有者句柄	
	wcscpy_s(szPathName, TEXT("new.bmp"));		// 定义预先的存储文件名
	ofn.lpstrFilter = TEXT("bmp文件(*.bmp)\0*.bmp\0所有文件(*.*)\0*.*\0\0");
	ofn.lpstrFile = szPathName;
	ofn.nMaxFile = sizeof(szPathName);
	ofn.lpstrTitle = TEXT("保存");
	ofn.Flags = OFN_OVERWRITEPROMPT;		// 覆盖提示

	BOOL bOk = GetSaveFileName(&ofn);
	if (!bOk)
	{
		return FALSE;
	}
	strPath = szPathName;

	return TRUE;
}

