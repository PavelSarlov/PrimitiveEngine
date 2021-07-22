#pragma once
#ifndef PRIMITIVEENGINE_H
#define PRIMITIVEENGINE_H

#include <stdint.h>
#include <Windows.h>
#include <stdio.h>

#define RES uint16_t
#define COORD int32_t
#define ECODE uint16_t
#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define DEFAULT_WCLASS L"pWindow"
#define DEFAULT_WNAME L"Primitive Engine"

class PrimitiveEngine
{
public:
	PrimitiveEngine();
	PrimitiveEngine(RES width, RES height);

	virtual void OnUserChange() = 0;
	virtual void UserInit() = 0;

	ECODE DrawPixel(COORD x, COORD y, COLORREF c);
	ECODE DrawLine(COORD x1, COORD y1, COORD x2, COORD y2, COLORREF c);
	ECODE DrawTriangle(COORD x1, COORD y1, COORD x2, COORD y2, COORD x3, COORD y3, COLORREF c);
	ECODE FillTriangle(COORD x1, COORD y1, COORD x2, COORD y2, COORD x3, COORD y3, COLORREF c);
	ECODE DrawCircle(COORD x1, COORD y1, COORD r, COLORREF c);
	ECODE FillCircle(COORD x1, COORD y1, COORD r, COLORREF c);

	ECODE Run();

private:
	ECODE FillBottomFlatTriangle(COORD x1, COORD y1, COORD x2, COORD y2, COORD x3, COORD y3, COLORREF c);
	ECODE FillTopFlatTriangle(COORD x1, COORD y1, COORD x2, COORD y2, COORD x3, COORD y3, COLORREF c);
	void SortBottomTopTriangle(COORD& x1, COORD& y1, COORD& x2, COORD& y2, COORD& x3, COORD& y3);

	static LRESULT CALLBACK ProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	RES width, height;
	HWND hwnd;
	HDC hdc;
	bool globalRunning;
};

#endif // !PRIMITIVEENGINE_H