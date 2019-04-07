#include "stdafx.h"
#include "resource.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <ostream>
#include <iostream>

//===============================================================
//===============================================================

//Function i*i - 3*i + 3

const double scale = 10;
const double a = 0.5 ;
const double b = 6;
const double c = 5;
bool down = false;
float x = 0;
float y = 0;
float x2 = 0;
float y2 = 0;

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

class Program
{
public:
	static void OnDraw(HDC hdc, HWND hWnd)
	{
		auto pen1 = CreatePen(PS_SOLID, 2, Colors::Black);
		auto pen2 = CreatePen(PS_SOLID, 1, RGB(255,0,0));
		auto pen3 = CreatePen(PS_DOT, 1, RGB(0, 100, 0));
		auto pen5 = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

		auto penBrush = LOGBRUSH{};
		penBrush.lbColor = Colors::Turquoise3;
		auto pen4 = ExtCreatePen(PS_GEOMETRIC | PS_DASH, 3, &penBrush, 0, nullptr);

		//===


		auto converter = MathToHdcConverter(hWnd);

	
		MoveToEx(hdc, converter.GetX(-300), converter.GetY(0), nullptr);
		LineTo(hdc, converter.GetX(300),converter.GetY(0));
		MoveToEx(hdc, converter.GetX(0), converter.GetY(-200), nullptr);
		LineTo(hdc, converter.GetX(0), converter.GetY(200));
		auto hOldPen = SelectObject(hdc, pen2);
		MoveToEx(hdc, converter.GetX(-200), converter.GetY(400), nullptr);
		for (int i = -200; i < 200; i++) {
			auto y = -2*i;
			LineTo(hdc, converter.GetX(i), converter.GetY(y));			
		}
		SelectObject(hdc, pen3);
		MoveToEx(hdc, converter.GetX(-200), converter.GetY(-a*200*-200 + b  * - 200 + c), nullptr);
		for (int i = -200; i < 200; i++) {
			auto y = a * i * i + b * i + c;
			LineTo(hdc, converter.GetX(i), converter.GetY(y));
		}
		SelectObject(hdc, pen5);
		MoveToEx(hdc, converter.GetX(-200), converter.GetY(sin(-200 ) + b), nullptr);
		for (int i = -200; i < 200; i++) {
			auto y = sin(i ) + b;
			LineTo(hdc, converter.GetX(i), converter.GetY(y));
		}

		if (down)
		{
			

		}


		

		auto r = 1;
		auto phi = M_PI / 4;
		//LineTo(hdc, converter.GetX(PolarToX(r, phi)), converter.GetY(PolarToY(r, phi)));

		//===

	//	SelectObject(hdc, hOldPen);
		DeleteObject(pen1);
		DeleteObject(pen2);
		DeleteObject(pen3);
		DeleteObject(pen4);
	}

private:
	static double PolarToX(double r, double phi)
	{
		return r*cos(phi); // TODO.
	}

	static double PolarToY(double r, double phi)
	{
		return r * sin(phi); // TODO.
	}
};

//===============================================================
//===============================================================

#define MAX_LOADSTRING 100

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
	RECT rcTmp;              
	
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
	switch (message)
	{




	case WM_LBUTTONDOWN:
	{
		down = true;
	    hdc = GetDC(hWnd);
		auto converter = MathToHdcConverter(hWnd);
		x = (LOWORD(lParam) - converter.GetClientRect().right / 2) / scale;
		y = -(HIWORD(lParam) - converter.GetClientRect().bottom / 2) / scale;

		 
		
		char str[40];
		sprintf_s(str, "x=%f y=%f", x, y);
		TextOutA(hdc, 1, 1, str, strlen(str));
		ReleaseDC(hWnd, hdc);
		break;
	}

	/*case WM_MOUSEMOVE :
	{
		if (wParam & MK_LBUTTON)
		{

			HDC hdc = GetDC(hWnd);
			auto converter = MathToHdcConverter(hWnd);
			x2 = (LOWORD(lParam) - converter.GetClientRect().right / 2) / scale;
			y2 = -(HIWORD(lParam) - converter.GetClientRect().bottom / 2) / scale;
			SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
			Rectangle(hdc, converter.GetX(x), converter.GetY(y), converter.GetX(x2), converter.GetY(y2));


			ReleaseDC(hWnd, hdc);


		}
		break;
	}*/

	case WM_MOUSEMOVE:

	{	// Draw a target rectangle or drag the bitmap rectangle,
		// depending on the status of the fDragRect flag.
		auto converter = MathToHdcConverter(hWnd);
		x2 = (LOWORD(lParam) - converter.GetClientRect().right / 2) / scale;
		y2 = -(HIWORD(lParam) - converter.GetClientRect().bottom / 2) / scale;
		hdc = GetDC(hWnd);
		if (wParam == MK_LBUTTON)
		{
			SetROP2(hdc, R2_NOTXORPEN);
			if (!IsRectEmpty(&rcTarget))
			{
				InvalidateRect(hWnd,&rcTarget,TRUE);
			}
			UpdateWindow(hWnd);
			if(x2 > x && y2 > y)
			{
				SetRect(&rcTarget, converter.GetX(x), converter.GetY(y2), converter.GetX(x2), converter.GetY(y));
			}
			else if(x2 < x && y2 < y)
				{
				SetRect(&rcTarget, converter.GetX(x2), converter.GetY(y), converter.GetX(x), converter.GetY(y2));
				}
			else if(x2 > x && y2 < y)
			{
				SetRect(&rcTarget, converter.GetX(x), converter.GetY(y), converter.GetX(x2), converter.GetY(y2));
			} else
			{
				SetRect(&rcTarget, converter.GetX(x2), converter.GetY(y2), converter.GetX(x), converter.GetY(y));
			}
			SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
			Rectangle(hdc, rcTarget.left, rcTarget.top,
				rcTarget.right, rcTarget.bottom);


		}
			
		
		break;
	}

	
	

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		Program::OnDraw(hdc, hWnd);
		
		EndPaint(hWnd, &ps);
		break;
	}
	

	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
