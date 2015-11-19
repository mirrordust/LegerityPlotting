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
