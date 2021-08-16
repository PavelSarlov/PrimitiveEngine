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
	virtual void onFocus() override;
	virtual void onKillFocus() override;

public:
	virtual void onKeyDown(USHORT key) override;
	virtual void onKeyUp(USHORT key) override;

	virtual void onMouseMove(const Point &mouse_pos) override;
	virtual void onLeftMouseDown(const Point &mouse_pos) override;
	virtual void onLeftMouseUp(const Point &mouse_pos) override;
	virtual void onRightMouseDown(const Point &mouse_pos) override;
	virtual void onRightMouseUp(const Point &mouse_pos) override;

public:
	void update();

private:
	SwapChain *m_swap_chain = nullptr;
	VertexBuffer *m_vb = nullptr;
	VertexShader *m_vs = nullptr;
	PixelShader *m_ps = nullptr;
	ConstantBuffer *m_cb = nullptr;
	IndexBuffer *m_ib = nullptr;

private:
	ULONG m_old_delta = 0;
	ULONG m_new_delta = 0;

	float m_delta_time = 0.0f;
	float m_delta_pos = 0.0f;
	float m_delta_scale = 0.0f;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_scale_cube = 1.0f;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	Matrix4x4 m_world_cam;
};

#endif