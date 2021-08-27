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
#ifndef SPACESHOOTERGAME_H
#define SPACESHOOTERGAME_H

#include "Prerequisites.h"
#include "Window.h"
#include "InputListener.h"
#include "Geometry.h"

#include <Windows.h>

class SpaceShooterGame : public Window, public InputListener
{
public:
	SpaceShooterGame();
	~SpaceShooterGame();

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
	void updateModel(const Vector3 &position, const Vector3 &rotation, const Vector3 &scale, const std::vector<MaterialPtr> &list_materials);
	void updateCamera();
	void updateThirdPersonCamera();
	void updateSkyBox();
	void updateLight();
	void updateSpaceship();
	void updateViewportProjection();

	void drawMesh(const MeshPtr &mesh, const std::vector<MaterialPtr> &list_materials);

private:
	SwapChainPtr m_swap_chain;

	TexturePtr m_sky_tex;
	TexturePtr m_spaceship_tex;
	TexturePtr m_asteroid_tex;

	MeshPtr m_sky_mesh;
	MeshPtr m_spaceship_mesh;
	MeshPtr m_asteroid_mesh;

	MaterialPtr m_base_mat;
	MaterialPtr m_sky_mat;
	MaterialPtr m_spaceship_mat;
	MaterialPtr m_asteroid_mat;

	std::vector<MaterialPtr> m_list_materials;

private:
	float zNear = 0.1f;
	float zFar = 5000.0f;

	ULONG m_old_delta = 0;
	ULONG m_new_delta = 0;
	float m_delta_time = 0.0f;

	float m_delta_mouse_x = 0.0f;
	float m_delta_mouse_y = 0.0f;

	Matrix4x4 m_light_rot_matrix;

	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	float m_upward = 0.0f;

	bool m_play_state = true;
	bool m_fullscreen = false;

	UINT m_frames = 0;
	float m_old_time = 0.0f;
	float m_time = 0.0f;

	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;
	Vector3 m_current_cam_rot;
	Vector3 m_cam_rot;
	Vector3 m_cam_pos;
	float m_current_cam_distance = 14.0f;
	float m_cam_distance = 14.0f;

	Vector3 m_current_spaceship_pos;
	Vector3 m_current_spaceship_rot;
	Vector3 m_spaceship_pos;
	Vector3 m_spaceship_rot;
	float m_spaceship_speed = 125.0f;
	bool m_spaceship_turbo_mode = false;

	Vector3 m_asteroids_pos[200];
	Vector3 m_asteroids_rot[200];
	Vector3 m_asteroids_scale[200];
};


#endif // !SPACESHOOTERGAME_H