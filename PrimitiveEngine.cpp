#include "PrimitiveEngine.h"
#include <algorithm>

PrimitiveEngine::PrimitiveEngine() : PrimitiveEngine(DEFAULT_WIDTH, DEFAULT_HEIGHT)
{

}

PrimitiveEngine::PrimitiveEngine(RES w, RES h)
{
	this->width = w;
	this->height = h;
	this->globalRunning = true;
}

ECODE PrimitiveEngine::Run()
{
	WNDCLASSEX wc{};
	wc.lpszClassName = DEFAULT_WCLASS;
	wc.hInstance = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.style = NULL;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = PrimitiveEngine::ProcessMessages;

	ATOM wClass = RegisterClassEx(&wc);
	if(!wClass)
	{
		fprintf(stderr, "%ld\n", GetLastError());
		fprintf(stderr, "%s\n", "Failed to create window class");
		return 1;
	}

	this->hwnd = CreateWindowEx(0, MAKEINTATOM(wClass), DEFAULT_WNAME,
		WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZE | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, nullptr, nullptr);
	if(!hwnd)
	{
		fprintf(stderr, "%ld\n", GetLastError());
		fprintf(stderr, "%s\n", "Failed to create window");
		return 2;
	}

	this->hdc = GetDC(hwnd);

	UserInit();

	ShowWindow(hwnd, SW_SHOWNORMAL);

	// window loop
	MSG msg{};
	while(globalRunning)
	{
		if(PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		this->OnUserChange();
	}

	return 0;
}

LRESULT CALLBACK PrimitiveEngine::ProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

ECODE PrimitiveEngine::DrawPixel(COORD x, COORD y, COLORREF c)
{
	if(!SetPixel(this->hdc, x, y, c))
	{
		return 1;
	}
	return 0;
}

ECODE PrimitiveEngine::DrawLine(COORD x1, COORD y1, COORD x2, COORD y2, COLORREF c)
{
	COORD dx = x2 - x1,
		dy = y2 - y1,
		a, b, p;

	if(abs(dx) >= abs(dy))
	{
		if(x1 > x2)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
			dx = -dx;
			dy = -dy;
		}

		COORD yi = 1;
		if(dy < 0)
		{
			yi = -1;
			dy = -dy;
		}

		a = 2 * dy;
		b = 2 * dx;
		p = a - dx;

		for(COORD x = x1 + 1, y = y1; x <= x2; x++)
		{
			if(DrawPixel(x, y, c))
			{
				return 1;
			}

			if(p >= 0)
			{
				y += yi;
				p -= b;
			}
			p += a;
		}
	}
	else
	{
		if(y1 > y2)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
			dx = -dx;
			dy = -dy;
		}

		COORD xi = 1;
		if(dx < 0)
		{
			xi = -1;
			dx = -dx;
		}

		a = 2 * dx;
		b = 2 * dy;
		p = a - dy;

		for(COORD x = x1, y = y1 + 1; y <= y2; y++)
		{
			if(DrawPixel(x, y, c))
			{
				return 1;
			}

			if(p >= 0)
			{
				x += xi;
				p -= b;
			}
			p += a;
		}
	}

	return 0;
}

ECODE PrimitiveEngine::DrawTriangle(COORD x1, COORD y1, COORD x2, COORD y2, COORD x3, COORD y3, COLORREF c)
{
	if(DrawLine(x1, y1, x2, y2, c) ||
		DrawLine(x2, y2, x3, y3, c) ||
		DrawLine(x3, y3, x1, y1, c))
	{
		return 1;
	}

	return 0;
}

ECODE PrimitiveEngine::DrawCircle(COORD x1, COORD y1, COORD r, COLORREF c)
{
	COORD e = -r,
		x = r,
		y = 0;

	while(y <= x)
	{
		DrawPixel(x1 + x, y1 + y, c);
		DrawPixel(x1 - x, y1 + y, c);
		DrawPixel(x1 + x, y1 - y, c);
		DrawPixel(x1 - x, y1 - y, c);
		DrawPixel(x1 + y, y1 + x, c);
		DrawPixel(x1 - y, y1 + x, c);
		DrawPixel(x1 + y, y1 - x, c);
		DrawPixel(x1 - y, y1 - x, c);

		e += 2 * y + 1;
		y++;

		if(e >= 0)
		{
			e -= 2 * x - 1;
			x--;
		}
	}

	return 0;
}

ECODE PrimitiveEngine::FillCircle(COORD x1, COORD y1, COORD r, COLORREF c)
{
	COORD e = -r,
		x = r,
		y = 0;

	while(y <= x)
	{
		DrawLine(x1 - x, y1 + y, x1 + x, y1 + y, c);
		DrawLine(x1 - x, y1 - y, x1 + x, y1 - y, c);
		DrawLine(x1 - y, y1 + x, x1 + y, y1 + x, c);
		DrawLine(x1 - y, y1 - x, x1 + y, y1 - x, c);

		e += 2 * y + 1;
		y++;

		if(e >= 0)
		{
			e -= 2 * x - 1;
			x--;
		}
	}

	return 0;
}

ECODE PrimitiveEngine::FillTriangle(COORD x1, COORD y1, COORD x2, COORD y2, COORD x3, COORD y3, COLORREF c)
{
	SortLeftRightTriangle(x1, y1, x2, y2, x3, y3);

	COORD dx2 = x2 - x1,
		dy2 = y2 - y1,
		dx3 = x3 - x1,
		dy3 = y3 - y1,
		curY2 = y1,
		curY3 = y1,
		a2, b2, a3, b3, p2, p3, yi2 = 1, yi3 = 1;

	if(dy2 < 0)
	{
		yi2 = -1;
		dy2 = -dy2;
	}

	if(dy3 < 0)
	{
		yi3 = -1;
		dy3 = -dy3;
	}

	if(abs(dx2) >= abs(dy2))
	{
		a2 = 2 * dy2;
		b2 = 2 * dx2;
		p2 = a2 - dx2;
	}
	else
	{
		a2 = 2 * dx2;
		b2 = 2 * dy2;
		p2 = a2 - dy2;
	}

	if(abs(dx3) >= abs(dy3))
	{
		a3 = 2 * dy3;
		b3 = 2 * dx3;
		p3 = a3 - dx3;
	}
	else
	{
		a3 = 2 * dx3;
		b3 = 2 * dy3;
		p3 = a3 - dy3;
	}

	for(COORD x = x1; x <= x3; x++)
	{
		if(DrawLine(x, curY2, x, curY3, c))
		{
			return 1;
		}

		if(x == x2 && curY2 == y2)
		{
			dx2 = x3 - x2;
			dy2 = y3 - y2;
			yi2 = 1;

			if(dy2 < 0)
			{
				yi2 = -1;
				dy2 = -dy2;
			}

			if(abs(dx2) >= abs(dy2))
			{
				a2 = 2 * dy2;
				b2 = 2 * dx2;
				p2 = a2 - dx2;
			}
			else
			{
				a2 = 2 * dx2;
				b2 = 2 * dy2;
				p2 = a2 - dy2;
			}
		}

		if(abs(dx2) >= abs(dy2))
		{
			if(p2 >= 0)
			{
				while(p2 >= 0)
				{
					curY2 += yi2;
					p2 -= b2 - a2;
				}
			}
			else
			{
				p2 += a2;
			}
		}
		else
		{
			if(p2 < 0)
			{
				curY2 += yi2;
				p2 += a2;
			}
			else
			{
				while(p2 >= 0)
				{
					curY2 += yi2;
					p2 -= b2 - a2;
				}
			}
		}

		if(abs(dx3) >= abs(dy3))
		{
			if(p3 >= 0)
			{
				while(p3 >= 0)
				{
					curY3 += yi3;
					p3 -= b3 - a3;
				}
			}
			else
			{
				p3 += a3;
			}
		}
		else
		{
			if(p3 < 0)
			{
				curY3 += yi3;
				p3 += a3;
			}
			else
			{
				while(p3 >= 0)
				{
					curY3 += yi3;
					p3 -= b3 - a3;
				}
			}
		}
	}

	return 0;
}

void PrimitiveEngine::SortLeftRightTriangle(COORD& x1, COORD& y1, COORD& x2, COORD& y2, COORD& x3, COORD& y3)
{
	if(x1 > x2 || (x1 == x2 && y1 > y2))
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	if(x1 > x3 || (x1 == x3 && y1 > y3))
	{
		std::swap(x1, x3);
		std::swap(y1, y3);
	}

	if(x2 > x3 || (x2 == x3 && y2 > y3))
	{
		std::swap(x2, x3);
		std::swap(y2, y3);
	}
}
