#include "FrameBufferDemo.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "InputSystem.h"
#include "Mesh.h"
#include "Material.h"
#include "MathUtils.h"

#include <time.h>

__declspec(align(16))
struct Constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4 m_cam_pos;
	Vector4 m_light_dir;
	Vector4 m_light_pos = Vector4(0, 1, 0, 0);
	float m_light_radius = 4.0f;
	float m_time = 0.0f;
};

FrameBufferDemo::FrameBufferDemo() : Window()
{}

FrameBufferDemo::~FrameBufferDemo()
{}

void FrameBufferDemo::onCreate()
{
	Window::onCreate();

	// subscribe for input events
	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	// create swap chain
	RECT rect = this->getClientWindowRect();
	this->m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	// create textures
	this->m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars_map.jpg");
	this->m_brick_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick_d.jpg");
	this->m_brick_normal_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick_n.jpg");

	// create meshes
	this->m_sphere_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	this->m_monitor_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\monitor.obj");

	// create materials
	this->m_base_mat = GraphicsEngine::get()->createMaterial(L"DirectionalLightVertexShader.hlsl", L"DirectionalLightPixelShader.hlsl");
	this->m_base_mat->setCullMode(CULL_MODE_BACK);

	this->m_sky_mat = GraphicsEngine::get()->createMaterial(L"SkyBoxVertexShader.hlsl", L"SkyBoxPixelShader.hlsl");
	this->m_sky_mat->addTexture(this->m_sky_tex);
	this->m_sky_mat->setCullMode(CULL_MODE_FRONT);

	this->m_brick_mat = GraphicsEngine::get()->createMaterial(L"DirLightBumpVS.hlsl", L"DirLightBumpPS.hlsl");
	this->m_brick_mat->addTexture(this->m_brick_tex);
	this->m_brick_mat->addTexture(this->m_brick_normal_tex);
	this->m_brick_mat->setCullMode(CULL_MODE_BACK);

	this->m_monitor_mat = GraphicsEngine::get()->createMaterial(this->m_base_mat);
	this->m_monitor_mat->addTexture(this->m_brick_tex);
	this->m_monitor_mat->setCullMode(CULL_MODE_BACK);

	this->m_screen_mat = GraphicsEngine::get()->createMaterial(this->m_base_mat);
	this->m_screen_mat->setCullMode(CULL_MODE_BACK);

	// init cam pos
	this->m_world_cam = Matrix4x4::translationMatrix(0, 0, -2);

	this->m_list_materials.reserve(32);

	this->m_mini_game.setWindowSize(Rect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top));
	this->m_mini_game.onCreate();

	this->m_screen_mat->addTexture(this->m_mini_game.getRenderTarget());
}

void FrameBufferDemo::onUpdate()
{
	Window::onUpdate();
	InputSystem::get()->update();

	this->m_mini_game.onUpdate();

	this->update();
	this->render();

	this->m_delta_mouse_x = 0.0f;
	this->m_delta_mouse_y = 0.0f;
}

void FrameBufferDemo::onDestroy()
{
	Window::onDestroy();
	this->m_swap_chain->setFullScreen(false, 1, 1);
}

void FrameBufferDemo::onFocus()
{
	InputSystem::get()->addListener(this);
}

void FrameBufferDemo::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void FrameBufferDemo::onResize()
{
	RECT rc = this->getClientWindowRect();
	this->m_swap_chain->resize(rc.right - rc.left, rc.bottom - rc.top);
	this->m_mini_game.setWindowSize(Rect(rc.right - rc.left, rc.bottom - rc.top));

	this->update();
	this->render();
}

void FrameBufferDemo::onKeyDown(USHORT key)
{
	if(this->m_play_state)
		this->m_mini_game.onKeyDown(key);
}

void FrameBufferDemo::onKeyUp(USHORT key)
{
	if(this->m_play_state)
		this->m_mini_game.onKeyUp(key);

	switch(key)
	{
	case VK_ESCAPE:
	{
		this->m_play_state = !this->m_play_state;
		break;
	}
	case 'F':
	{
		this->m_fullscreen = !this->m_fullscreen;
		RECT size_screen = this->getSizeScreen();
		this->m_swap_chain->setFullScreen(this->m_fullscreen, size_screen.right, size_screen.bottom);
		break;
	}
	}
}

void FrameBufferDemo::onMouseMove(const POINT &mouse_pos)
{
	RECT win_size = this->getClientWindowRect();

	int width = (win_size.right - win_size.left);
	int height = (win_size.bottom - win_size.top);

	if(!this->m_play_state)
	{
		this->m_delta_mouse_x = (float)(mouse_pos.x - (win_size.left + (width / 2.0f)));
		this->m_delta_mouse_y = (float)(mouse_pos.y - (win_size.top + (height / 2.0f)));
	}

	InputSystem::get()->setCursorPos({ win_size.left + (LONG)(width / 2.0f), win_size.top + (LONG)(height / 2.0f) });

	if(this->m_play_state)
		this->m_mini_game.onMouseMove(mouse_pos);
}

void FrameBufferDemo::onLeftMouseDown(const POINT &mouse_pos)
{
	if(this->m_play_state)
		this->m_mini_game.onLeftMouseDown(mouse_pos);
}

void FrameBufferDemo::onLeftMouseUp(const POINT &mouse_pos)
{
	if(this->m_play_state)
		this->m_mini_game.onLeftMouseUp(mouse_pos);
}

void FrameBufferDemo::onRightMouseDown(const POINT &mouse_pos)
{
	if(this->m_play_state)
		this->m_mini_game.onRightMouseDown(mouse_pos);
}

void FrameBufferDemo::onRightMouseUp(const POINT &mouse_pos)
{
	if(this->m_play_state)
		this->m_mini_game.onRightMouseUp(mouse_pos);
}

void FrameBufferDemo::onMouseWheelUp(const POINT &mouse_pos, const short &wheel_delta)
{
	if(this->m_play_state)
		this->m_mini_game.onMouseWheelUp(mouse_pos, wheel_delta);
}

void FrameBufferDemo::onMouseWheelDown(const POINT &mouse_pos, const short &wheel_delta)
{
	if(this->m_play_state)
		this->m_mini_game.onMouseWheelDown(mouse_pos, wheel_delta);
}

void FrameBufferDemo::render()
{
	// clear render target
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	// set viewport of render target in which we will draw
	RECT rect = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewPortSize(rect.right - rect.left, rect.bottom - rect.top);

	this->m_list_materials.clear();
	this->m_list_materials.push_back(this->m_monitor_mat);
	this->m_list_materials.push_back(this->m_screen_mat);
	updateModel(Vector3(), Vector3(0, 3.14f, 0), Vector3(1, 1, 1), this->m_list_materials);
	this->drawMesh(this->m_monitor_mesh, this->m_list_materials);

	// draw skybox
	this->m_list_materials.clear();
	this->m_list_materials.push_back(this->m_sky_mat);
	this->drawMesh(this->m_sphere_mesh, this->m_list_materials);

	// swap back and front buffers
	this->m_swap_chain->present(true);

	// timing
	this->m_old_delta = this->m_new_delta;
	this->m_new_delta = (ULONG)::GetTickCount64();
	//this->m_delta_time = this->m_old_delta ? (this->m_new_delta - this->m_old_delta) / 1000.0f : 0.0f;
	this->m_delta_time = 1.0f / 60.0f;
	this->m_time += this->m_delta_time;

	// fps
	this->m_frames++;
	if(this->m_time - this->m_old_time >= 1.0f)
	{
		wchar_t title[256];
		swprintf_s(title, 256, L"FPS: %d", (int)(this->m_frames / (this->m_time - this->m_old_time)));
		::SetWindowTextW(this->m_hwnd, title);
		this->m_old_time = this->m_time;
		this->m_frames = 0;
	}
}

void FrameBufferDemo::update()
{
	this->updateThirdPersonCamera();
	this->updateLight();
	this->updateSkyBox();
}

void FrameBufferDemo::updateModel(const Vector3 &position, const Vector3 &rotation, const Vector3 &scale, const std::vector<MaterialPtr> &list_materials)
{
	Constant cc = {};
	cc.m_world = Matrix4x4::identityMatrix() *
		Matrix4x4::scaleMatrix(scale) *
		Matrix4x4::rotationX(rotation.m_x) *
		Matrix4x4::rotationY(rotation.m_y) *
		Matrix4x4::rotationZ(rotation.m_z) *
		Matrix4x4::translationMatrix(position);
	cc.m_view = this->m_view_cam;
	cc.m_proj = this->m_proj_cam;
	cc.m_cam_pos = this->m_world_cam.getTranslation();

	cc.m_light_dir = m_light_rot_matrix.getDirectionZ();
	cc.m_light_pos = Vector4(0, 1, 0);
	cc.m_light_radius = 0.0f;
	cc.m_time = this->m_time;

	for(auto &m : list_materials)
	{
		m->setData(&cc, sizeof(cc));
	}
}

void FrameBufferDemo::updateCamera()
{
	this->m_cam_rot.m_x += this->m_delta_mouse_y * this->m_delta_time * 0.1f;
	this->m_cam_rot.m_y += this->m_delta_mouse_x * this->m_delta_time * 0.1f;

	Matrix4x4 world_cam = Matrix4x4::identityMatrix();

	world_cam *= Matrix4x4::rotationX(this->m_cam_rot.m_x);
	world_cam *= Matrix4x4::rotationY(this->m_cam_rot.m_y);

	Vector3 new_pos = this->m_world_cam.getTranslation() +
		world_cam.getDirectionZ() * this->m_forward * 0.07f +
		world_cam.getDirectionX() * this->m_rightward * 0.07f +
		world_cam.getDirectionY() * this->m_upward * 0.07f;

	world_cam.setTranslation(new_pos);
	this->m_world_cam = world_cam;
	world_cam = world_cam.inversedMatrix();

	this->m_view_cam = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	this->m_proj_cam.setPerspectiveFovPH(1.57f, ((float)width / (float)height), zNear, zFar);
}

void FrameBufferDemo::updateThirdPersonCamera()
{
	this->m_cam_rot.m_x += this->m_delta_mouse_y * this->m_delta_time * 0.1f;
	this->m_cam_rot.m_y += this->m_delta_mouse_x * this->m_delta_time * 0.1f;

	this->m_cam_rot.m_x = max(-1.57f, min(1.57f, this->m_cam_rot.m_x));

	this->m_current_cam_rot = Vector3::lerp(this->m_current_cam_rot, this->m_cam_rot, 3.0f * this->m_delta_time);

	Matrix4x4 world_cam = Matrix4x4::identityMatrix();
	world_cam *= Matrix4x4::rotationX(this->m_current_cam_rot.m_x);
	world_cam *= Matrix4x4::rotationY(this->m_current_cam_rot.m_y);

	this->m_current_cam_distance = lerp(this->m_current_cam_distance, this->m_cam_distance, 2.0f * this->m_delta_time);

	Vector3 new_pos = this->m_cam_pos +
		world_cam.getDirectionZ() * (-this->m_current_cam_distance);

	world_cam.setTranslation(new_pos);
	this->m_world_cam = world_cam;
	this->m_view_cam = world_cam.inversedMatrix();

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	this->m_proj_cam.setPerspectiveFovPH(1.57f, ((float)width / (float)height), zNear, zFar);
}

void FrameBufferDemo::updateSkyBox()
{
	Constant cc = {};

	cc.m_world = Matrix4x4::identityMatrix();
	cc.m_world.setScale(zFar, zFar, zFar);
	cc.m_world.setTranslation(this->m_world_cam.getTranslation());
	cc.m_view = this->m_view_cam;
	cc.m_proj = this->m_proj_cam;

	this->m_sky_mat->setData(&cc, sizeof(cc));
}

void FrameBufferDemo::updateLight()
{
	this->m_light_rot_matrix = Matrix4x4::identityMatrix();

	this->m_light_rot_matrix *= Matrix4x4::rotationX(-0.707f);
	this->m_light_rot_matrix *= Matrix4x4::rotationY(2.14f);
}

void FrameBufferDemo::drawMesh(const MeshPtr &mesh, const std::vector<MaterialPtr> &list_materials)
{
	// set the list of vertices
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	// set the list of indices
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	for(size_t m = 0; m < mesh->getNumMaterialSlots() && m < list_materials.size(); m++)
	{
		MaterialSlot mat = mesh->getMaterialSlot(m);

		GraphicsEngine::get()->setMaterial(list_materials[m]);

		// draw the object
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList((UINT)mat.num_indices, 0, (UINT)mat.start_index);
	}
}