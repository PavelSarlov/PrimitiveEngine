#pragma once

#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "Prerequisites.h"

#include <d3d11.h>

class DeviceContext;

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem *system);
	~SwapChain();

public:
	bool present(bool vsync);
	void resize(UINT width, UINT height);
	void setFullScreen(bool fullscreen, UINT width, UINT height);

private:
	void reloadBuffers(UINT width, UINT height);

private:
	IDXGISwapChain *m_swap_chain = nullptr;
	ID3D11RenderTargetView *m_rtv = nullptr;
	ID3D11DepthStencilView *m_dsv = nullptr;
	RenderSystem *m_system = nullptr;

private:
	friend class DeviceContext;
};

#endif