#pragma once

#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "PrimitiveEngine.h"

#include <d3d11.h>

class DeviceContext;

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

public:
	bool Init(HWND hwnd, UINT width, UINT height);
	bool Release();
	bool Present(bool vsync);

private:
	IDXGISwapChain *m_swap_chain = nullptr;
	ID3D11RenderTargetView *m_rtv = nullptr;

private:
	friend class DeviceContext;
};

#endif