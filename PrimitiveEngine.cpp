#include "PrimitiveEngine.h"

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

ECODE DrawLine(COORD x1, COORD y1, COORD x2, COORD y2, COLORREF c)
{
	return 0;
}

ECODE PrimitiveEngine::DrawTriangle()
{
	return 0;
}

ECODE PrimitiveEngine::DrawCircle()
{
	return 0;
}