#pragma once

#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "Prerequisites.h"
#include "RenderSystem.h"

#include <d3d11.h>
#include <exception>

class DeviceContext;

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem *system);
	~SwapChain();

public:
	bool present(bool vsync);

private:
	IDXGISwapChain *m_swap_chain = nullptr;
	ID3D11RenderTargetView *m_rtv = nullptr;
	RenderSystem *m_system = nullptr;

private:
	friend class DeviceContext;
};

#endif