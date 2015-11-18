#include "stdafx.h"
#include "utilities.h"


using namespace std;

void plot(HDC hdc, TCHAR* expression, POINT origin,
	float XrangeLeft, float XrangeRight, float XplottingScale, float YplottingScale)
{
	queue<valsym> rpn = postfix(expression);

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
		::wsprintf(buf, L"=======> %d和%d", points[10].x, points[10].y);
		TextOut(hdc, 300, 300, buf, ::wcslen(buf));
	}

}

calres calculate(queue<valsym> rpn, float x)
{
	calres result;
	result.valid = true;
	stack<double> st_result;
	int j = rpn.size();
	while (j--)
	{
		valsym t_vs = rpn.front();
		rpn.pop();
		if (t_vs.type == 1)
		{
			t_vs.type = 0;
			t_vs.val = x;
		}
		rpn.push(t_vs);
	}
	while (!rpn.empty())
	{
		valsym t_vs = rpn.front();
		rpn.pop();
		if (t_vs.type == 0)
		{
			st_result.push(t_vs.val);
		}
		else
		{
			TCHAR op = t_vs.sym[0];
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
					return result;
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
queue<valsym> postfix(TCHAR* expression)
{
	//TCHAR ch, ch_pre = '#';
	TCHAR** e2 = tokenizer(expression);
	/*for (unsigned int i = 0; i < wcslen(expression);)
	{
	ch = *(expression+i);
	i++;
	if (isdigit(ch_pre) && ch == 'x')
	{
	TCHAR x = '*';
	expression = expression.insert(i - 1, &x, 1);
	ch = expression.at(++i);
	ch_pre = 'x';
	}
	else
	{
	ch_pre = ch;
	}

	}*/
	map<TCHAR, int> isp;
	isp.insert(pair<TCHAR, int>('#', 0));
	isp.insert(pair<TCHAR, int>('(', 1));
	isp.insert(pair<TCHAR, int>('*', 5));
	isp.insert(pair<TCHAR, int>('/', 5));
	isp.insert(pair<TCHAR, int>('%', 5));
	isp.insert(pair<TCHAR, int>('+', 3));
	isp.insert(pair<TCHAR, int>('-', 3));
	isp.insert(pair<TCHAR, int>(')', 8));
	isp.insert(pair<TCHAR, int>('^', 6));
	map<TCHAR, int> icp;
	icp.insert(pair<TCHAR, int>('#', 0));
	icp.insert(pair<TCHAR, int>('(', 8));
	icp.insert(pair<TCHAR, int>('*', 4));
	icp.insert(pair<TCHAR, int>('/', 4));
	icp.insert(pair<TCHAR, int>('%', 4));
	icp.insert(pair<TCHAR, int>('+', 2));
	icp.insert(pair<TCHAR, int>('-', 2));
	icp.insert(pair<TCHAR, int>(')', 1));
	icp.insert(pair<TCHAR, int>('^', 7));

	//	string t_str = expression + '#';
	INT index = 0;
	stack<TCHAR> symbol;
	queue<valsym> result;
	valsym t_valsym;
	//	string s;
	TCHAR ch = '#';
	TCHAR* buf;
	symbol.push(ch);
	buf = e2[index++];
	while (e2[index] != NULL)
	{
		switch (judgeBuf(buf))
		{
		case 0:
		{
			t_valsym.type = 0;
			wstring s(buf);
			t_valsym.val = _wtof(buf);
			result.push(t_valsym);
			buf = e2[index++];
			/*s = string(&ch, 1);
			ch = t_str.at(index++);
			while (isdigit(ch) || ch == '.')
			{
			s += ch;
			ch = t_str.at(index++);
			}
			index -= 1;
			result.push(s);
			ch = t_str.at(index++);*/
			break;
		}
		case 1:
		{
			t_valsym.type = 1;
			t_valsym.sym[0] = 'x';
			t_valsym.sym[1] = '\0';
			result.push(t_valsym);
			buf = e2[index++];
			/*s = string(&ch, 1);
			result.push(s);
			ch = t_str.at(index++);*/
			break;
		}
		case -1:
		{
			ch = buf[0];
			TCHAR& symbol_top = symbol.top();
			if (isp[symbol_top] < icp[ch])
			{
				symbol.push(ch);
				buf = e2[index++];
			}
			else if (isp[symbol_top] > icp[ch])
			{
				TCHAR& op = symbol.top();
				symbol.pop();
				t_valsym.type = -1;
				t_valsym.sym[0] = op;
				t_valsym.sym[1] = '\0';
				//s = string(&op, 1);
				result.push(t_valsym);
			}
			else
			{
				TCHAR& op = symbol.top();
				symbol.pop();
				if (op == '(')
				{
					buf = e2[index++];
					//ch = t_str.at(index++);
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
int judgeX(TCHAR ch)
{

	if (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5'
		|| ch == '6' || ch == '7' || ch == '8' || ch == '9')
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

int judgeBuf(TCHAR* buf)
{
	if (judgeX(buf[0]) == -1)
	{
		if (buf[1] != '\0' && judgeX(buf[1]) == 0)
			return 0;
		else
			return -1;
	}
	else if (judgeX(buf[0]) == 0)
	{
		return 0;
	}
	else if (judgeX(buf[0]) == 1)
	{
		return 1;
	}
	else
	{
		return -2;
	}
}

TCHAR** tokenizer(TCHAR* expression)
{
	TCHAR** e2 = new TCHAR*[wcslen(expression) * 2 - 1];
	TCHAR bu[20];
	wsprintf(bu, L"==>%d\n", wcslen(expression));
	OutputDebugString(bu);
	TCHAR now, pre;
	pre = '#';
	int tokennum = 0;
	const INT bufsize = 20;
	for (INT i = 0; i < wcslen(expression);)
	{
		TCHAR buf[bufsize];
		INT j = 0;
		now = expression[i];
		INT judge_x = judgeX(now);
		if (judge_x == 0)
		{
			BOOL p_n = TRUE;
			if (i >= 2 && expression[i - 2] == '(' && expression[i - 1] == '-')
			{
				p_n = FALSE;
			}
			if (!p_n)
			{
				buf[j++] = '-';
			}
			buf[j++] = now;
			i++;
			now = expression[i];
			while (judgeX(now) == 0 || now == '.')
			{
				buf[j++] = now;
				i++;
				now = expression[i];
			}
			pre = expression[i - 1];
			buf[j] = '\0';
			e2[tokennum++] = buf;
		}
		else if (judge_x == 1)
		{
			if (judgeX(pre) == 0)
			{
				buf[0] = '*';
				buf[1] = '\0';
				e2[tokennum++] = buf;
			}
			buf[0] = 'x';
			buf[1] = '\0';
			e2[tokennum++] = buf;
			pre = now;
			i++;
			now = expression[i];

		}
		else
		{
			buf[0] = now;
			buf[1] = '\0';
			e2[tokennum++] = buf;
			pre = now;
			i++;
			now = expression[i];

		}
	}
	e2[tokennum] = NULL;
	return e2;
}