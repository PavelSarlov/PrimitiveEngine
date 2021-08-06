#include "Window.h"


Window::Window()
{}

Window::~Window()
{}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:
	{
		// collect window...
		Window *window = (Window *)((LPCREATESTRUCT)lParam)->lpCreateParams;
		// ...and store for later lookup
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
		window->SetHWND(hwnd);
		window->OnCreate();
		break;
	}
	case WM_DESTROY:
	{
		Window *window = (Window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->OnDestroy();
		::PostQuitMessage(0);
		break;
	}
	default:
	{
		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}

	return NULL;
}

bool Window::Init()
{
	return this->Init(DEFAULT_WCLASS, DEFAULT_WNAME, DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

bool Window::Init(LPCWSTR className, LPCWSTR winName, UINT width, UINT height)
{
	WNDCLASSEX wc;
	wc.hInstance = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.style = NULL;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.cbSize = sizeof(wc);
	wc.lpszClassName = DEFAULT_WCLASS;
	wc.lpszMenuName = DEFAULT_WMENU;
	wc.lpfnWndProc = WinProc;

	if(!::RegisterClassEx(&wc))
	{
		fprintf(stderr, "%ld\n", GetLastError());
		fprintf(stderr, "%s\n", "Failed to create window class");
		return false;
	}

	/*if(!window)
	{
		window = this;
	}*/

	this->m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, className, winName,
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, nullptr, this);

	if(!this->m_hwnd)
	{
		fprintf(stderr, "%ld\n", GetLastError());
		fprintf(stderr, "%s\n", "Failed to create window");
		return false;
	}

	::ShowWindow(this->m_hwnd, SW_SHOW);
	::UpdateWindow(this->m_hwnd);

	this->m_is_running = true;
	return true;
}

bool Window::Process()
{
	MSG msg;

	this->OnUpdate();

	while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(1);

	return true;
}


bool Window::Release()
{
	if(!::DestroyWindow(this->m_hwnd))
	{
		fprintf(stderr, "%ld\n", GetLastError());
		fprintf(stderr, "%s\n", "Failed to destroy window");
		return false;
	}

	return true;
}

bool Window::IsRunning()
{
	return this->m_is_running;
}

RECT Window::GetClientWindowRect()
{
	RECT rect;
	::GetClientRect(this->m_hwnd, &rect);
	return rect;
}

void Window::SetHWND(HWND hwnd)
{
	this->m_hwnd = hwnd;
}

void Window::OnCreate()
{}

void Window::OnUpdate()
{}

void Window::OnDestroy()
{
	this->m_is_running = false;
}