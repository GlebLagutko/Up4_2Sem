#include "stdafx.h"
#include "resource.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <ostream>
#include <iostream>
#include <commdlg.h>


//===============================================================
//===============================================================

//Function i*i - 3*i + 3

double scale = 10;
double a = 0.7;
double b = 3;
double c = -7;
bool down = false;
bool changes = false;
float x = 0;
float y = 0;
float x2 = 0;
float y2 = 0;
float x3 = 0;
float y3 = 0;
const float epsS = 2.0;
const float epsP = 2.0;
DWORD rgbPar = RGB(0, 200, 100);
DWORD rgbSin = RGB(100, 0, 200);
HMENU hMenubar;
HMENU hMenu;



namespace Colors
{
	static COLORREF FromHex(int hex)
	{
		auto r = (hex & 0xFF0000) >> 16;
		auto g = (hex & 0x00FF00) >> 8;
		auto b = (hex & 0x0000FF) >> 0;
		return RGB(r, g, b);
	}

	static const COLORREF Firebrick = FromHex(0xB22222);
	static const COLORREF Goldenrod1 = FromHex(0xFFC125);
	static const COLORREF SpringGreen3 = FromHex(0x00CD66);
	static const COLORREF Black = FromHex(0x000000);
	static const COLORREF White = FromHex(0xFFFFFF);
	static const COLORREF SteelBlue2 = FromHex(0x5CACEE);
	static const COLORREF SlateBlue = FromHex(0x6A5ACD);
	static const COLORREF Sienna1 = FromHex(0xFF8247);
	static const COLORREF Turquoise3 = FromHex(0x00C5CD);
}



class MathToHdcConverter
{
	HWND _hWnd;

public:
	explicit MathToHdcConverter(HWND hWnd) : _hWnd(hWnd) {  }

	int GetX(const double x) const
	{
		return GetCenterX() + x * GetScale();
	}

	int GetY(const double y) const
	{
		return GetCenterY() - y * GetScale();
	}

	int GetSize(const double size) const
	{
		return size * GetScale();
	}

	RECT GetClientRect() const
	{
		RECT clientRect;
		::GetClientRect(_hWnd, &clientRect);
		return clientRect;
	}

private:
	int GetCenterX() const
	{
		return GetClientRect().right / 2;
	}

	int GetCenterY() const
	{
		return GetClientRect().bottom / 2;
	}

	double GetScale() const
	{

		return scale;
	}
};

double parab(double x, double a, double b, double c) {
	auto y = a * (x + x3 )* (x + x3) + b * (x + x3) + c;
	return y;
}

double Sin(double x, double a, double b) {
	return (sin(a * (x + x3)) + b);
}

class Program
{
public:
	static void OnDraw(HDC hdc, HWND hWnd)
	{
		auto pen1 = CreatePen(PS_SOLID, 2, Colors::Black);
		
		auto pen3 = CreatePen(PS_SOLID, 4, rgbPar);
		auto pen5 = CreatePen(PS_DOT, 1,rgbSin);
	
		

		auto converter = MathToHdcConverter(hWnd);

		MoveToEx(hdc, converter.GetX(-300), converter.GetY(0), nullptr);
		LineTo(hdc, converter.GetX(300), converter.GetY(0));
		MoveToEx(hdc, converter.GetX(0), converter.GetY(-200), nullptr);
		LineTo(hdc, converter.GetX(0), converter.GetY(200));
		
		SelectObject(hdc, pen3);
		MoveToEx(hdc, converter.GetX(-200), converter.GetY(-a * 200 * -200 + b * -200 + c), nullptr);
		for (int i = -200; i < 200; i++) {
			auto y = parab(i, a, b, c);
			LineTo(hdc, converter.GetX(i), converter.GetY(y));
		}
		SelectObject(hdc, pen5);
		MoveToEx(hdc, converter.GetX(-200), converter.GetY(sin(-200 * a) + b), nullptr);
		for (int i = -200; i < 200; i++) {
			auto y = Sin(i, a, b);
			LineTo(hdc, converter.GetX(i), converter.GetY(y));
		}

		DeleteObject(pen1);
	
		DeleteObject(pen3);
		DeleteObject(pen5);
	}

};


//===============================================================
//===============================================================

#define MAX_LOADSTRING 100
HWND hWnd;
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING] = L"Drawing";
WCHAR szWindowClass[MAX_LOADSTRING] = L"Drawing_App";

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

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDRAWING));

	MSG msg;

	// Main message loop:
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



ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = nullptr;



	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	RECT rc;
	static HDC hdcCompat;    // DC for copying bitmap  
	static RECT rcBmp;       // rectangle that encloses bitmap  
	static RECT rcTarget;    // rectangle to receive bitmap  
	static RECT rcClient;    // client-area rectangle  
	static BOOL fDragRect;   // TRUE if bitmap rect. is dragged  
	static HBITMAP hbmp;     // handle of bitmap to display  
	static HBRUSH hbrBkgnd;  // handle of background-color brush  
	static COLORREF crBkgnd; // color of client-area background  
	static POINT pt;
	static HPEN hpenDot;
	HWND hWndEditA = NULL;
	HWND hWndEditB;
	HWND hWndEditC;
	
	switch (message)
	{
	case WM_CREATE :
		{
		hdc = GetDC(hWnd);
		hMenubar = CreateMenu();
	    hMenu = CreateMenu();

		AppendMenuW(hMenu, MF_STRING, 0, L"&Color Par");
		AppendMenuW(hMenu, MF_STRING, 1, L"&Color Sin");
		AppendMenuW(hMenu, MF_STRING, 2, L"&Param");
		AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Edit");

		SetMenu(hWnd, hMenu);
		SetMenu(hWnd, hMenubar);

		hWndEditA = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE, 850, 10, 40,
			20, hWnd, NULL, NULL, NULL);
		hWndEditB = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE, 850, 40, 40,
			20, hWnd, NULL, NULL, NULL);
		hWndEditC = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE, 850, 70, 40,
			20, hWnd, NULL, NULL, NULL);
		
	/*	auto TextBoxA = CreateWindow(L"EDIT",
			L" ",
			WS_BORDER | WS_CHILD | WS_VISIBLE,
			850, 10, 50, 20,
			hWnd, (HMENU)1, NULL, NULL);
		auto TextBoxB = CreateWindow(L"EDIT",
			L" ",
			WS_BORDER | WS_CHILD | WS_VISIBLE,
			850, 40, 50, 20,
			hWnd, (HMENU)1, NULL, NULL);
		auto TextBoxC = CreateWindow(L"EDIT",
			L" ",
			WS_BORDER | WS_CHILD | WS_VISIBLE,
			850, 70, 50, 20,
			hWnd, (HMENU)1, NULL, NULL);*/
		
		
		
		}
		break;
	case WM_LBUTTONDOWN:
	{
		down = true;
		hdc = GetDC(hWnd);
		auto converter = MathToHdcConverter(hWnd);
		x = (LOWORD(lParam) - converter.GetClientRect().right / 2) / scale;
		y = -(HIWORD(lParam) - converter.GetClientRect().bottom / 2) / scale;
		char str[100];
		//TODO :: y
		double yParab = parab(x, a, b, c);
		double ySin = Sin(x, a, b);
	
		if (y + epsP >= yParab && y - epsP <= yParab) {
			sprintf_s(str, "x=%f Par y=%f", x, yParab);
			TextOutA(hdc, 1, 1, str, strlen(str));
		}
		if (y + epsS >= ySin && y - epsS <= ySin) {
			sprintf_s(str, "x=%f Sin y=%f", x, ySin);
			TextOutA(hdc, 1, 20, str, strlen(str));
		}



		ReleaseDC(hWnd, hdc);
		
	}
	break;
	case WM_MOUSEMOVE:

	{	// Draw a target rectangle or drag the bitmap rectangle,
		// depending on the status of the fDragRect flag.
		if (wParam == MK_LBUTTON)
		{
			auto converter = MathToHdcConverter(hWnd);
			x2 = (LOWORD(lParam) - converter.GetClientRect().right / 2) / scale;
			y2 = -(HIWORD(lParam) - converter.GetClientRect().bottom / 2) / scale;
			hdc = GetDC(hWnd);
			SetROP2(hdc, R2_NOTXORPEN);
			auto pen2 = CreatePen(PS_SOLID, 1, RGB(0, 0, 1000));
			SelectObject(hdc, pen2);
			if (!IsRectEmpty(&rcTarget))
			{
				InvalidateRect(hWnd, &rcTarget, TRUE);
			}
			UpdateWindow(hWnd);
			if (x2 > x && y2 > y)
			{
				SetRect(&rcTarget, converter.GetX(x), converter.GetY(y2), converter.GetX(x2), converter.GetY(y));
			}
			else if (x2 < x && y2 < y)
			{
				SetRect(&rcTarget, converter.GetX(x2), converter.GetY(y), converter.GetX(x), converter.GetY(y2));
			}
			else if (x2 > x && y2 < y)
			{
				SetRect(&rcTarget, converter.GetX(x), converter.GetY(y), converter.GetX(x2), converter.GetY(y2));
			}
			else
			{
				SetRect(&rcTarget, converter.GetX(x2), converter.GetY(y2), converter.GetX(x), converter.GetY(y));
			}
			SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
			Rectangle(hdc, rcTarget.left, rcTarget.top,
				rcTarget.right, rcTarget.bottom);
		}

		
	}
	break;
	case WM_LBUTTONUP:
	{
		if (down)
		{
			if (x != x2 && y != y2 && x2 != 0 && y2 != 0)
			{
				hdc = GetDC(hWnd);
				auto converter = MathToHdcConverter(hWnd);
				
				
			}
		}
		InvalidateRect(hWnd, &rcTarget, TRUE);
	
	}
	break;
	case WM_COMMAND:
	{
		hdc = GetDC(hWnd);
		switch (wParam)
		{
		case 0:
		{
			CHOOSECOLOR cc = { 0 };
			cc.lStructSize = sizeof(cc);
			COLORREF cust_colors[16] = { 0 };
			cc.lpCustColors = cust_colors;

			if (ChooseColor(&cc)) {
				rgbPar = cc.rgbResult;
			
				Program::OnDraw(hdc, hWnd);
			}

		}
		break;
		case 1:
		{
			CHOOSECOLOR cc = { 0 };
			cc.lStructSize = sizeof(cc);
			COLORREF cust_colors[16] = { 0 };
			cc.lpCustColors = cust_colors;

			if (ChooseColor(&cc)) {
				rgbSin = cc.rgbResult;
				Program::OnDraw(hdc, hWnd);
			}
			break;
		}
		case 2 :
		{
			
		}
		break;
		}


	}
	break;
	case WM_KEYDOWN :
		{
		
		}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hdc = GetDC(hWnd);
		

		hdc = BeginPaint(hWnd, &ps);
		Program::OnDraw(hdc, hWnd);


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
