// MIT License
// 
// C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)
// 
// Copyright (c) 2019-2021, PardCode
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#ifndef APPWINDOW_H
#define APPWINDOW_H

#include "Prerequisites.h"
#include "Window.h"
#include "InputListener.h"
#include "Geometry.h"

#include <Windows.h>

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
	virtual void onResize() override;

public:
	virtual void onKeyDown(USHORT key) override;
	virtual void onKeyUp(USHORT key) override;

	virtual void onMouseMove(const POINT &mouse_pos) override;
	virtual void onLeftMouseDown(const POINT &mouse_pos) override;
	virtual void onLeftMouseUp(const POINT &mouse_pos) override;
	virtual void onRightMouseDown(const POINT &mouse_pos) override;
	virtual void onRightMouseUp(const POINT &mouse_pos) override;
	virtual void onMouseWheelUp(const POINT &mouse_pos, const  short &wheel_delta) override;
	virtual void onMouseWheelDown(const POINT &mouse_pos, const short &wheel_delta) override;

public:
	void render();
	void update();
	void updateModel(const Vector3 &position, const std::vector<MaterialPtr> &list_materials);
	void updateCamera();
	void updateSkyBox();
	void updateLight();

	void drawMesh(const MeshPtr &mesh, const std::vector<MaterialPtr> &list_materials);

private:
	SwapChainPtr m_swap_chain;
	VertexBufferPtr m_vb;
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	PixelShaderPtr m_sky_ps;
	ConstantBufferPtr m_cb;
	ConstantBufferPtr m_sky_cb;
	IndexBufferPtr m_ib;

	TexturePtr m_earth_color_tex;
	TexturePtr m_earth_night_tex;
	TexturePtr m_earth_spec_tex;
	TexturePtr m_sky_tex;
	TexturePtr m_cloud_tex;
	TexturePtr m_wall_tex;
	TexturePtr m_sand_tex;
	TexturePtr m_barrel_tex;
	TexturePtr m_brick_tex;
	TexturePtr m_windows_tex;
	TexturePtr m_wood_tex;

	MeshPtr m_mesh;
	MeshPtr m_sky_mesh;
	MeshPtr m_torus_mesh;
	MeshPtr m_suzanne_mesh;
	MeshPtr m_plane_mesh;
	MeshPtr m_terrain_mesh;
	MeshPtr m_house_mesh;

	MaterialPtr m_mat;
	MaterialPtr m_earth_mat;
	MaterialPtr m_sky_mat;
	MaterialPtr m_terrain_mat;
	MaterialPtr m_barrel_mat;
	MaterialPtr m_brick_mat;
	MaterialPtr m_windows_mat;
	MaterialPtr m_wood_mat;

	std::vector<MaterialPtr> m_list_materials;

private:
	float zNear = 0.1f;
	float zFar = 100.0f;

	ULONG m_old_delta = 0;
	ULONG m_new_delta = 0;

	float m_delta_time = 0.0f;
	float m_delta_pos = 0.0f;
	float m_delta_scale = 0.0f;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_light_rot_y = 0.0f;
	float m_light_radius = 500.0f;
	float m_attenuation = 2.0f;
	Vector4 m_light_pos;

	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	float m_upward = 0.0f;

	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;

	bool m_play_state = true;
	bool m_fullscreen = false;

	int m_frames = 0;
	float m_old_time = 0.0f;
	float m_time = 0.0f;
};

#endif