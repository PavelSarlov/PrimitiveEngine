// MIT License
// 
// C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)
// 
// Copyright (c) 2019-2021, PardCode
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Window.h"

#include <exception>

#define DEFAULT_WIDTH (UINT)1024
#define DEFAULT_HEIGHT (UINT)768
#define DEFAULT_WCLASS L"Window"
#define DEFAULT_WNAME L"New Window"
#define DEFAULT_WMENU L"Menu"

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_MOUSEWHEEL:
	{
		// event fired on mouse scrolling
		Window *window = (Window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if(window)
		{
			POINT current_mouse_pos = {};
			::GetCursorPos(&current_mouse_pos);
			short wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam);

			if(wheel_delta > 0)
			{
				window->onMouseWheelUp(current_mouse_pos, wheel_delta);
			}
			else
			{
				window->onMouseWheelDown(current_mouse_pos, wheel_delta);
			}
		}

		break;
	}
	case WM_CREATE:
	{
		// event fired on window creation
		break;
	}
	case WM_SIZE:
	{
		// event fired on window resizing
		Window *window = (Window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if(window) window->onResize();
		break;
	}
	case WM_SETFOCUS:
	{
		// event fired on window getting focus
		Window *window = (Window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if(window) window->onFocus();
		break;
	}
	case WM_KILLFOCUS:
	{
		// event fired on window losing
		Window *window = (Window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->onKillFocus();
		break;
	}
	case WM_DESTROY:
	{
		// event fired on window closing
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

	this->m_hwnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, className, winName,
		WS_CAPTION | WS_SYSMENU | WS_OVERLAPPEDWINDOW,
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
	RECT rc;
	::GetClientRect(this->m_hwnd, &rc);
	::ClientToScreen(this->m_hwnd, (LPPOINT)&rc.left);
	::ClientToScreen(this->m_hwnd, (LPPOINT)&rc.right);
	return rc;
}

RECT Window::getWindowRect()
{
	RECT rc;
	::GetWindowRect(this->m_hwnd, &rc);
	return rc;
}

RECT Window::getSizeScreen()
{
	RECT rc;
	rc.right = ::GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);
	return rc;
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

void Window::onResize()
{}

void Window::onMouseWheelUp(const POINT &mouse_pos, const short &wheel_delta)
{}

void Window::onMouseWheelDown(const POINT &mouse_pos, const short &wheel_delta)
{}