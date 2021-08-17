#include "Window.h"

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:
	{
		break;
	}
	case WM_SETFOCUS:
	{
		Window *window = (Window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if(window) window->onFocus();
		break;
	}
	case WM_KILLFOCUS:
	{
		Window *window = (Window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}
	case WM_DESTROY:
	{
		Window *window = (Window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onDestroy();
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

Window::Window() : Window(DEFAULT_WCLASS, DEFAULT_WNAME, DEFAULT_WIDTH, DEFAULT_HEIGHT)
{}

Window::Window(LPCWSTR className, LPCWSTR winName, UINT width, UINT height)
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
		throw std::exception("Window class registration failed");
	}

	/*if(!window)
	{
		window = this;
	}*/

	this->m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, className, winName,
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, nullptr, nullptr);

	if(!this->m_hwnd)
	{
		throw std::exception("Window creation failed");
	}

	::ShowWindow(this->m_hwnd, SW_SHOW);
	::UpdateWindow(this->m_hwnd);

	this->m_is_running = true;
}

Window::~Window()
{}

bool Window::broadcast()
{
	MSG msg;

	if(!this->m_is_init)
	{
		SetWindowLongPtr(this->m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		this->onCreate();
		this->m_is_init = true;
	}

	this->onUpdate();

	while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(1);

	return true;
}

bool Window::isRunning()
{
	if(this->m_is_running) this->broadcast();
	return this->m_is_running;
}

RECT Window::getClientWindowRect()
{
	RECT rect;
	::GetClientRect(this->m_hwnd, &rect);
	return rect;
}

void Window::onCreate()
{}

void Window::onUpdate()
{}

void Window::onDestroy()
{
	this->m_is_running = false;
}

void Window::onFocus()
{}

void Window::onKillFocus()
{}
