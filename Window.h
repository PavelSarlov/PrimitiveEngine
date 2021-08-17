#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <exception>

#define DEFAULT_WIDTH (UINT)1024
#define DEFAULT_HEIGHT (UINT)768
#define DEFAULT_WCLASS L"Window"
#define DEFAULT_WNAME L"New Window"
#define DEFAULT_WMENU L"Menu"

class Window
{
public:
	Window();
	Window(LPCWSTR className, LPCWSTR winName, UINT width, UINT height);
	~Window();

public:
	bool isRunning();
	RECT getClientWindowRect();

public:
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

private:
	bool broadcast();

protected:
	HWND m_hwnd = nullptr;
	bool m_is_running = false;
	bool m_is_init = false;
};


#endif // !WINDOW_H