#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>

class Window
{
public:
	Window();
	Window(LPCWSTR className, LPCWSTR winName, UINT width, UINT height);
	~Window();

public:
	bool isRunning();
	RECT getClientWindowRect();
	RECT getWindowRect();
	RECT getSizeScreen();

public:
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onResize();
	virtual void onMouseWheelUp(const POINT &mouse_pos, const short &wheel_delta);
	virtual void onMouseWheelDown(const POINT &mouse_pos, const short &wheel_delta);

private:
	bool broadcast();

protected:
	HWND m_hwnd = nullptr;
	bool m_is_running = false;
	bool m_is_init = false;
};


#endif // !WINDOW_H