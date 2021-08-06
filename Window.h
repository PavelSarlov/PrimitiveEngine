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
#define DEFAULT_WMENU L""

class Window
{
public:
	Window();
	~Window();

public:
	bool Init();
	bool Init(LPCWSTR className, LPCWSTR winName, UINT width, UINT height);
	bool Process();
	bool Release();
	bool IsRunning();
	RECT GetClientWindowRect();
	void SetHWND(HWND hwnd);

public:
	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnDestroy();

protected:
	HWND m_hwnd = nullptr;
	bool m_is_running = false;
};


#endif // !WINDOW_H