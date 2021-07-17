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
	if (!wClass)
	{
		fprintf(stderr, "%ld\n", GetLastError());
		fprintf(stderr, "%s\n", "Failed to create window class");
		return 1;
	}

	this->hwnd = CreateWindowEx(0, MAKEINTATOM(wClass), DEFAULT_WNAME,
		WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZE | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, nullptr, nullptr);
	if (!hwnd)
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
	while (globalRunning)
	{
		if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
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
	switch (msg)
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
	if (!SetPixel(this->hdc, x, y, c))
	{
		return 1;
	}
	return 0;
}

ECODE PrimitiveEngine::DrawLine(COORD x1, COORD y1, COORD x2, COORD y2, COLORREF c)
{
	DrawPixel(x1, y1, c);

	COORD dx = x2 - x1,
		dy = y2 - y1,
		a, b, p;

	if (abs(dx) >= abs(dy))
	{
		if (x1 > x2)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
			dx = x2 - x1;
			dy = y2 - y1;
		}

		COORD yi = 1;
		if (dy < 0)
		{
			yi = -1;
			dy = -dy;
		}

		a = 2 * dy;
		b = 2 * dx;
		p = a - dx;

		for (COORD x = x1 + 1, y = y1; x <= x2; x++)
		{
			if (p > 0)
			{
				y += yi;
				p -= b;
			}
			p += a;
			DrawPixel(x, y, c);
		}
	}
	else
	{
		if (y1 > y2)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
			dx = x2 - x1;
			dy = y2 - y1;
		}

		COORD xi = 1;
		if (dx < 0)
		{
			xi = -1;
			dx = -dx;
		}

		a = 2 * dx;
		b = 2 * dy;
		p = a - dy;

		for (COORD x = x1, y = y1 + 1; y <= y2; y++)
		{
			if (p > 0)
			{
				x += xi;
				p -= b;
			}
			p += a;
			DrawPixel(x, y, c);
		}
	}

	return 0;
}

ECODE PrimitiveEngine::DrawTriangle(COORD x1, COORD y1, COORD x2, COORD y2, COORD x3, COORD y3, COLORREF c)
{
	DrawLine(x1, y1, x2, y2, c);
	DrawLine(x2, y2, x3, y3, c);
	DrawLine(x3, y3, x1, y1, c);

	return 0;
}

ECODE PrimitiveEngine::DrawCircle(COORD x1, COORD y1, COORD r, COLORREF c)
{
	return 0;
}