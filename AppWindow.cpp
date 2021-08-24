#include "AppWindow.h"
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

#include <future>

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

AppWindow::AppWindow() : Window()
{}

AppWindow::~AppWindow()
{}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(!this->m_play_state);

	// create textures
	this->m_wall_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\wall.jpg");
	this->m_bricks_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");
	this->m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars_map.jpg");
	this->m_earth_color_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\earth_color.jpg");

	// create meshes
	this->m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\scene.obj");
	this->m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	this->m_torus_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\torus.obj");
	this->m_suzanne_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\suzanne.obj");
	this->m_plane_mesh= GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\plane.obj");

	// create swap chain
	RECT rect = this->getClientWindowRect();
	this->m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	this->m_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"PointLightPixelShader.hlsl");
	this->m_mat->addTexture(this->m_wall_tex);
	this->m_mat->setCullMode(CULL_MODE_BACK);

	this->m_earth_mat = GraphicsEngine::get()->createMaterial(m_mat);
	this->m_earth_mat->removeTexture(0);
	this->m_earth_mat->addTexture(this->m_earth_color_tex);
	this->m_earth_mat->setCullMode(CULL_MODE_BACK);

	this->m_bricks_mat = GraphicsEngine::get()->createMaterial(m_mat);
	this->m_bricks_mat->removeTexture(0);
	this->m_bricks_mat->addTexture(this->m_bricks_tex);
	this->m_bricks_mat->setCullMode(CULL_MODE_BACK);

	this->m_sky_mat = GraphicsEngine::get()->createMaterial(L"PointLightVertexShader.hlsl", L"SkyBoxShader.hlsl");
	this->m_sky_mat->addTexture(this->m_sky_tex);
	this->m_sky_mat->setCullMode(CULL_MODE_FRONT);

	// init cam pos
	this->m_world_cam = Matrix4x4::translationMatrix(0, 0, -2);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	InputSystem::get()->update();
	this->render();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	this->m_swap_chain->setFullScreen(false, 1, 1);
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onResize()
{
	RECT rc = this->getClientWindowRect();
	this->m_swap_chain->resize(rc.right - rc.left, rc.bottom - rc.top);
	this->render();
}

void AppWindow::onKeyDown(USHORT key)
{
	switch(key)
	{
	case 'W':
	{
		this->m_forward = 1.0f;
		break;
	}
	case 'S':
	{
		this->m_forward = -1.0f;
		break;
	}
	case 'A':
	{
		this->m_rightward = -1.0f;
		break;
	}
	case 'D':
	{
		this->m_rightward = 1.0f;
		break;
	}
	case VK_SPACE:
	{
		this->m_upward = 1.0f;
		break;
	}
	case 'X':
	{
		this->m_upward = -1.0f;
		break;
	}
	}
}

void AppWindow::onKeyUp(USHORT key)
{
	switch(key)
	{
	case 'W':
	case 'S':
	{
		this->m_forward = 0.0f;
		break;
	}
	case 'A':
	case 'D':
	{
		this->m_rightward = 0.0f;
		break;
	}
	case VK_SPACE:
	case 'X':
	{
		this->m_upward = 0.0f;
		break;
	}
	case VK_ESCAPE:
	{
		this->m_play_state = !this->m_play_state;
		InputSystem::get()->showCursor(!this->m_play_state);
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

void AppWindow::onMouseMove(const POINT &mouse_pos)
{
	if(!this->m_play_state) return;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	this->m_rot_x += (mouse_pos.y - (height / 2.0f)) * this->m_delta_time * 0.1f;
	this->m_rot_y += (mouse_pos.x - (width / 2.0f)) * this->m_delta_time * 0.1f;

	InputSystem::get()->setCursorPos({ (LONG)(width / 2.0f), (LONG)(height / 2.0f) });
}

void AppWindow::onLeftMouseDown(const POINT &mouse_pos)
{}

void AppWindow::onLeftMouseUp(const POINT &mouse_pos)
{}

void AppWindow::onRightMouseDown(const POINT &mouse_pos)
{}

void AppWindow::onRightMouseUp(const POINT &mouse_pos)
{}

void AppWindow::onMouseWheelUp(const POINT &mouse_pos, const short &wheel_delta)
{
	this->m_light_radius += this->m_delta_time * wheel_delta * 0.1f;
}

void AppWindow::onMouseWheelDown(const POINT &mouse_pos, const short &wheel_delta)
{
	this->m_light_radius += this->m_delta_time * wheel_delta * 0.1f;
}

void AppWindow::render()
{
	// clear render target
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	// set viewport of render target in which we will draw
	RECT rect = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewPortSize(rect.right - rect.left, rect.bottom - rect.top);

	// compute transform matrices
	this->update();

	// render model
	this->updateModel(Vector3(0, 0, 0), this->m_mat);
	this->drawMesh(this->m_torus_mesh, this->m_mat);

	// render model
	this->updateModel(Vector3(4, 0, 0), this->m_earth_mat);
	this->drawMesh(this->m_sky_mesh, this->m_earth_mat);

	// render model
	this->updateModel(Vector3(-4, 0, 0), this->m_bricks_mat);
	this->drawMesh(this->m_suzanne_mesh, this->m_bricks_mat);

	this->updateModel(Vector3(0, -2, 0), this->m_mat);
	this->drawMesh(this->m_plane_mesh, this->m_mat);

	// render skybox
	this->drawMesh(this->m_sky_mesh, this->m_sky_mat);

	// swap back and front buffers
	this->m_swap_chain->present(true);

	// timing
	this->m_old_delta = this->m_new_delta;
	this->m_new_delta = (ULONG)::GetTickCount64();
	this->m_delta_time = this->m_old_delta ? (this->m_new_delta - this->m_old_delta) / 1000.0f : 0.0f;
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

void AppWindow::update()
{
	this->updateCamera();
	this->updateLight();
	this->updateSkyBox();
}

void AppWindow::updateModel(const Vector3 &position, const MaterialPtr &material)
{
	Constant cc = {};

	Matrix4x4 m_light_rot_matrix = Matrix4x4::rotationY(this->m_light_rot_y);

	cc.m_world = Matrix4x4::identityMatrix();
	cc.m_world.setTranslation(position);
	cc.m_view = this->m_view_cam;
	cc.m_proj = this->m_proj_cam;
	cc.m_cam_pos = this->m_world_cam.getTranslation();

	cc.m_light_dir = m_light_rot_matrix.getDirectionZ();
	cc.m_light_pos = this->m_light_pos;
	cc.m_light_radius = this->m_light_radius;
	cc.m_time = this->m_time;

	material->setData(&cc, sizeof(cc));
}

void AppWindow::updateCamera()
{
	Matrix4x4 world_cam = Matrix4x4::identityMatrix();

	world_cam *= Matrix4x4::rotationX(m_rot_x);
	world_cam *= Matrix4x4::rotationY(m_rot_y);

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

void AppWindow::updateSkyBox()
{
	Constant cc = {};

	cc.m_world = Matrix4x4::identityMatrix();
	cc.m_world.setScale(zFar, zFar, zFar);
	cc.m_world.setTranslation(this->m_world_cam.getTranslation());
	cc.m_view = this->m_view_cam;
	cc.m_proj = this->m_proj_cam;

	this->m_sky_mat->setData(&cc, sizeof(cc));
}

void AppWindow::updateLight()
{
	this->m_light_rot_y += 1.57f * this->m_delta_time;
	float dist_from_origin = 3.0f;

	this->m_light_pos = Vector4(cosf(m_light_rot_y) * dist_from_origin, 2.0f, sinf(m_light_rot_y) * dist_from_origin);
}

void AppWindow::drawMesh(const MeshPtr &mesh, const MaterialPtr &material)
{
	GraphicsEngine::get()->setMaterial(material);

	// set the list of vertices
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	// set the list of indices
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	// draw the object
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
}