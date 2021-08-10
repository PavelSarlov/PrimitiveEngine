#pragma once
#ifndef APPWINDOW_H
#define APPWINDOW_H

#include "Window.h"
#include "PrimitiveEngine.h"
#include "Geometry.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "PixelShader.h"

class AppWindow : public Window
{
public:
	AppWindow();
	~AppWindow();

public:
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

private:
	SwapChain *m_swap_chain = nullptr;
	VertexBuffer *m_vb = nullptr;
	VertexShader *m_vs = nullptr;
	PixelShader *m_ps = nullptr;
};

#endif