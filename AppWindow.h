#pragma once
#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <Windows.h>

#include "Window.h"
#include "PrimitiveEngine.h"
#include "Geometry.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "InputListener.h"
#include "InputSystem.h"

class AppWindow : public Window, public InputListener
{
public:
	AppWindow();
	~AppWindow();

public:
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

public:
	virtual void onKeyDown(USHORT key) override;
	virtual void onKeyUp(USHORT key) override;

public:
	void updateQuadPosition();

private:
	SwapChain *m_swap_chain = nullptr;
	VertexBuffer *m_vb = nullptr;
	VertexShader *m_vs = nullptr;
	PixelShader *m_ps = nullptr;
	ConstantBuffer *m_cb = nullptr;
	IndexBuffer *m_ib = nullptr;

private:
	float m_old_delta = 0.0f;
	float m_new_delta = 0.0f;
	float m_delta_time = 0.0f;
	float m_delta_pos = 0.0f;
	float m_delta_scale = 0.0f;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;
};

#endif