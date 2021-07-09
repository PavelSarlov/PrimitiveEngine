#include <stdio.h>
#include <iostream>
#include "VectorMatrix.h"
#include <Windows.h>

//int globalRunning = 1;
//int width = 800, height = 600;
//LPCWSTR CLASS_NAME = L"TestClass";
//LPCWSTR WINDOW_NAME = L"Window Test";
//
//LRESULT CALLBACK ProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//
//void PaintWindow(HWND hwnd, POINTS p);
//
//int main()
//{
//	// register window
//	WNDCLASSEX wc{};
//	wc.lpszClassName = CLASS_NAME;
//	wc.hInstance = nullptr;
//	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
//	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
//	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
//	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
//	wc.style = NULL;
//	wc.cbClsExtra = NULL;
//	wc.cbWndExtra = NULL;
//	wc.cbSize = sizeof(wc);
//	wc.lpfnWndProc = ProcessMessages;
//
//	ATOM wClass = RegisterClassEx(&wc);
//	if(!wClass)
//	{
//		fprintf(stderr, "%ld\n", GetLastError());
//		fprintf(stderr, "%s\n", "Failed to create window class");
//		return 1;
//	}
//
//	HWND hwnd = CreateWindowEx(0, MAKEINTATOM(wClass), WINDOW_NAME,
//		WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZE | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX,
//		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
//		nullptr, nullptr, nullptr, nullptr);
//	if(!hwnd)
//	{
//		fprintf(stderr, "%ld\n", GetLastError());
//		fprintf(stderr, "%s\n", "Failed to create window");
//		return 1;
//	}
//
//	ShowWindow(hwnd, SW_SHOWNORMAL);
//
//	// window loop
//	MSG msg{};
//	while(globalRunning)
//	{
//		while(PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
//		{
//			TranslateMessage(&msg);
//			DispatchMessageW(&msg);
//		}
//
//
//	}
//
//	return 0;
//}
//
//void PaintWindow(HWND hwnd, POINTS p)
//{
//	fprintf(stderr, "%d %d\n", p.x, p.y);
//	HDC hdc = GetDC(hwnd);
//	if(!SetPixel(hdc, p.x, p.y, RGB(0xFF, 0xFF, 0xFF)))
//	{
//		fprintf(stderr, "%ld\n", GetLastError());
//		fprintf(stderr, "%s\n", "Drawing failed");
//	}
//}
//
//LRESULT CALLBACK ProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	switch(msg)
//	{
//	case WM_CLOSE:
//		DestroyWindow(hwnd);
//		return 0;
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	case WM_LBUTTONDOWN:
//		POINTS p = MAKEPOINTS(lParam);
//		PaintWindow(hwnd, p);
//		return 0;
//	default:
//		return DefWindowProc(hwnd, msg, wParam, lParam);
//	}
//}

#include "GameTest.h"

int main()
{
	GameTest game;
	game.Run();
}