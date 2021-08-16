#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

#define RES uint16_t
#define COORD int32_t
#define ECODE uint16_t
#define DEFAULT_WIDTH (UINT)1024
#define DEFAULT_HEIGHT (UINT)768
#define DEFAULT_WCLASS L"Window"
#define DEFAULT_WNAME L"New Window"
#define DEFAULT_WMENU L"Menu"

class Window
{
public:
	Window();
	~Window();

public:
	bool init();
	bool init(LPCWSTR className, LPCWSTR winName, UINT width, UINT height);
	bool broadcast();
	bool release();
	bool isRunning();
	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

public:
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

protected:
	HWND m_hwnd = nullptr;
	bool m_is_running = false;
};


#endif // !WINDOW_H