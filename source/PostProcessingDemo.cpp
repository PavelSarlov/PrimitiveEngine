#include "PostProcessingDemo.h"
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

__declspec(align(16))
struct DistortionEffectData
{
	float m_distortion_level = 0.0f;
};

PostProcessingDemo::PostProcessingDemo() : Window()
{}

PostProcessingDemo::~PostProcessingDemo()
{}

void PostProcessingDemo::onCreate()
{
	Window::onCreate();

	// subscribe for input events
	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(!this->m_play_state);

	// create swap chain
	RECT rect = this->getClientWindowRect();
	this->m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	srand((UINT)time(NULL));

	for(UINT i = 0; i < 200; i++)
	{
		this->m_asteroids_pos[i] = Vector3((float)(rand() % 4000 + (-2000)), (float)(rand() % 4000 + (-2000)), (float)(rand() % 4000 + (-2000)));
		this->m_asteroids_rot[i] = Vector3((rand() % 628) / 100.0f, (rand() % 628) / 100.0f, (rand() % 628) / 100.0f);
		float scale = (float)(rand() % 20 + (6));
		this->m_asteroids_scale[i] = Vector3(scale, scale, scale);
	}

	// create textures
	this->m_sky_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars_map.jpg");
	this->m_spaceship_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\spaceship.jpg");
	this->m_asteroid_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\asteroid.jpg");

	// create meshes
	this->m_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\sphere.obj");
	this->m_spaceship_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\spaceship.obj");
	this->m_asteroid_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\asteroid.obj");

	VertexMesh quad_vertex_list[] =
	{
		VertexMesh(Vector3(-1,-1,0), Vector2(0,1), Vector3(), Vector3(),Vector3()),
		VertexMesh(Vector3(-1, 1,0), Vector2(0,0), Vector3(), Vector3(),Vector3()),
		VertexMesh(Vector3(1, 1,0), Vector2(1,0), Vector3(), Vector3(),Vector3()),
		VertexMesh(Vector3(1,-1,0), Vector2(1,1), Vector3(), Vector3(),Vector3()),
	};

	unsigned int quad_index_list[] =
	{
		0,1,2,
		2,3,0
	};

	MaterialSlot quad_mat_slots[] =
	{
		{0, 6, 0}
	};

	this->m_quad_mesh = GraphicsEngine::get()->getMeshManager()->createMesh(quad_vertex_list, 4, quad_index_list, 6, quad_mat_slots, 1);

	// create materials
	this->m_base_mat = GraphicsEngine::get()->createMaterial(L"DirectionalLightVertexShader.hlsl", L"DirectionalLightPixelShader.hlsl");
	this->m_base_mat->setCullMode(CULL_MODE_BACK);

	this->m_sky_mat = GraphicsEngine::get()->createMaterial(L"SkyBoxVertexShader.hlsl", L"SkyBoxPixelShader.hlsl");
	this->m_sky_mat->addTexture(this->m_sky_tex);
	this->m_sky_mat->setCullMode(CULL_MODE_FRONT);

	this->m_spaceship_mat = GraphicsEngine::get()->createMaterial(this->m_base_mat);
	this->m_spaceship_mat->addTexture(this->m_spaceship_tex);
	this->m_spaceship_mat->setCullMode(CULL_MODE_BACK);

	this->m_asteroid_mat = GraphicsEngine::get()->createMaterial(this->m_base_mat);
	this->m_asteroid_mat->addTexture(this->m_asteroid_tex);
	this->m_asteroid_mat->setCullMode(CULL_MODE_BACK);

	this->m_post_process_mat = GraphicsEngine::get()->createMaterial(L"PostProcessVS.hlsl", L"DistortionEffect.hlsl");
	this->m_post_process_mat->setCullMode(CULL_MODE_BACK);

	// init cam pos
	this->m_world_cam = Matrix4x4::translationMatrix(0, 0, -2);

	this->m_list_materials.reserve(32);

	this->m_render_target = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(rect.right - rect.left, rect.bottom - rect.top), Texture::Type::RENDER_TARGET);
	this->m_depth_stencil = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(rect.right - rect.left, rect.bottom - rect.top), Texture::Type::DEPTH_STENCIL);

	this->m_post_process_mat->addTexture(this->m_render_target);
}

void PostProcessingDemo::onUpdate()
{
	Window::onUpdate();
	InputSystem::get()->update();

	this->update();
	this->render();

	this->m_delta_mouse_x = 0.0f;
	this->m_delta_mouse_y = 0.0f;
}

void PostProcessingDemo::onDestroy()
{
	Window::onDestroy();
	this->m_swap_chain->setFullScreen(false, 1, 1);
}

void PostProcessingDemo::onFocus()
{
	InputSystem::get()->addListener(this);
}

void PostProcessingDemo::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void PostProcessingDemo::onResize()
{
	RECT rc = this->getClientWindowRect();
	this->m_swap_chain->resize(rc.right - rc.left, rc.bottom - rc.top);

	this->m_render_target = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(rc.right - rc.left, rc.bottom - rc.top), Texture::Type::RENDER_TARGET);
	this->m_depth_stencil = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(rc.right - rc.left, rc.bottom - rc.top), Texture::Type::DEPTH_STENCIL);

	this->m_post_process_mat->removeTexture(0);
	this->m_post_process_mat->addTexture(this->m_render_target);

	this->update();
	this->render();
}

void PostProcessingDemo::onKeyDown(USHORT key)
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
	case VK_SHIFT:
	{
		this->m_spaceship_turbo_mode = true;
		break;
	}
	}
}

void PostProcessingDemo::onKeyUp(USHORT key)
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
	case VK_SHIFT:
	{
		this->m_spaceship_turbo_mode = false;
		break;
	}
	}
}

void PostProcessingDemo::onMouseMove(const POINT &mouse_pos)
{
	if(!this->m_play_state) return;

	RECT win_size = this->getClientWindowRect();

	int width = (win_size.right - win_size.left);
	int height = (win_size.bottom - win_size.top);

	this->m_delta_mouse_x = (float)(mouse_pos.x - (win_size.left + (width / 2.0f)));
	this->m_delta_mouse_y = (float)(mouse_pos.y - (win_size.top + (height / 2.0f)));

	InputSystem::get()->setCursorPos({ win_size.left + (LONG)(width / 2.0f), win_size.top + (LONG)(height / 2.0f) });
}

void PostProcessingDemo::onLeftMouseDown(const POINT &mouse_pos)
{}

void PostProcessingDemo::onLeftMouseUp(const POINT &mouse_pos)
{}

void PostProcessingDemo::onRightMouseDown(const POINT &mouse_pos)
{}

void PostProcessingDemo::onRightMouseUp(const POINT &mouse_pos)
{}

void PostProcessingDemo::onMouseWheelUp(const POINT &mouse_pos, const short &wheel_delta)
{}

void PostProcessingDemo::onMouseWheelDown(const POINT &mouse_pos, const short &wheel_delta)
{}

void PostProcessingDemo::render()
{
	// render scene to render target
	//------------------------------------------

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_render_target, 0, 0.3f, 0.4f, 1);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearDepthStencil(this->m_depth_stencil);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setRenderTarget(this->m_render_target, this->m_depth_stencil);

	// set viewport of render target in which we will draw
	Rect viewport_size = this->m_render_target->getSize();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewPortSize(viewport_size.m_width, viewport_size.m_height);

	// draw spaceship
	this->m_list_materials.clear();
	this->m_list_materials.push_back(this->m_spaceship_mat);
	this->updateModel(this->m_current_spaceship_pos, this->m_current_spaceship_rot, Vector3(1, 1, 1), this->m_list_materials);
	this->drawMesh(this->m_spaceship_mesh, this->m_list_materials);

	// draw asteroids
	this->m_list_materials.clear();
	this->m_list_materials.push_back(this->m_asteroid_mat);
	for(UINT i = 0; i < 200; i++)
	{
		this->updateModel(this->m_asteroids_pos[i], this->m_asteroids_rot[i], this->m_asteroids_scale[i], this->m_list_materials);
		this->drawMesh(this->m_asteroid_mesh, this->m_list_materials);
	}

	// draw skybox
	this->m_list_materials.clear();
	this->m_list_materials.push_back(this->m_sky_mat);
	this->drawMesh(this->m_sky_mesh, this->m_list_materials);

	//------------------------------------------
	//------------------------------------------

	// clear render target
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	// set viewport of render target in which we will draw
	RECT rect = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewPortSize(rect.right - rect.left, rect.bottom - rect.top);

	DistortionEffectData effect_data;
	effect_data.m_distortion_level = this->m_distortion_level;

	this->m_list_materials.clear();
	this->m_list_materials.push_back(this->m_post_process_mat);
	this->m_post_process_mat->setData(&effect_data, sizeof(effect_data));
	this->drawMesh(this->m_quad_mesh, this->m_list_materials);

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

void PostProcessingDemo::update()
{
	this->updateSpaceship();
	this->updateThirdPersonCamera();
	this->updateViewportProjection();
	this->updateLight();
	this->updateSkyBox();
}

void PostProcessingDemo::updateModel(const Vector3 &position, const Vector3 &rotation, const Vector3 &scale, const std::vector<MaterialPtr> &list_materials)
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

void PostProcessingDemo::updateCamera()
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
	this->m_view_cam = world_cam.inversedMatrix();
}

void PostProcessingDemo::updateThirdPersonCamera()
{
	this->m_cam_rot.m_x += this->m_delta_mouse_y * this->m_delta_time * 0.1f;
	this->m_cam_rot.m_y += this->m_delta_mouse_x * this->m_delta_time * 0.1f;

	this->m_cam_rot.m_x = max(-1.57f, min(1.57f, this->m_cam_rot.m_x));

	this->m_current_cam_rot = Vector3::lerp(this->m_current_cam_rot, this->m_cam_rot, 3.0f * this->m_delta_time);

	Matrix4x4 world_cam = Matrix4x4::identityMatrix();
	world_cam *= Matrix4x4::rotationX(this->m_current_cam_rot.m_x);
	world_cam *= Matrix4x4::rotationY(this->m_current_cam_rot.m_y);

	if(this->m_forward > 0.0f)
	{
		this->m_cam_distance = this->m_spaceship_turbo_mode ? 19.0f : 16.0f;
	}
	else if(this->m_forward < 0.0f)
	{
		this->m_cam_distance = this->m_spaceship_turbo_mode ? 9.0f : 5.0f;
	}
	else
	{
		this->m_cam_distance = 14.0f;
	}

	this->m_current_cam_distance = lerp(this->m_current_cam_distance, this->m_cam_distance, 2.0f * this->m_delta_time);

	this->m_cam_pos = this->m_current_spaceship_pos;

	Vector3 new_pos = this->m_cam_pos +
		world_cam.getDirectionZ() * (-this->m_current_cam_distance) +
		/*world_cam.getDirectionX() * (-this->m_cam_distance) +*/
		world_cam.getDirectionY() * (5.0f);

	world_cam.setTranslation(new_pos);
	this->m_world_cam = world_cam;
	this->m_view_cam = world_cam.inversedMatrix();
}

void PostProcessingDemo::updateSkyBox()
{
	Constant cc = {};

	cc.m_world = Matrix4x4::identityMatrix();
	cc.m_world.setScale(zFar, zFar, zFar);
	cc.m_world.setTranslation(this->m_world_cam.getTranslation());
	cc.m_view = this->m_view_cam;
	cc.m_proj = this->m_proj_cam;

	this->m_sky_mat->setData(&cc, sizeof(cc));
}

void PostProcessingDemo::updateLight()
{
	this->m_light_rot_matrix = Matrix4x4::identityMatrix();

	this->m_light_rot_matrix *= Matrix4x4::rotationX(-0.707f);
	this->m_light_rot_matrix *= Matrix4x4::rotationY(0.707f);
}

void PostProcessingDemo::updateSpaceship()
{
	this->m_spaceship_rot.m_x += this->m_delta_mouse_y * this->m_delta_time * 0.1f;
	this->m_spaceship_rot.m_y += this->m_delta_mouse_x * this->m_delta_time * 0.1f;

	this->m_spaceship_rot.m_x = max(-1.57f, min(1.57f, this->m_spaceship_rot.m_x));

	this->m_current_spaceship_rot = Vector3::lerp(this->m_current_spaceship_rot, this->m_spaceship_rot, 5.0f * this->m_delta_time);

	Matrix4x4 world_model = Matrix4x4::identityMatrix();
	world_model *= Matrix4x4::rotationX(this->m_current_spaceship_rot.m_x);
	world_model *= Matrix4x4::rotationY(this->m_current_spaceship_rot.m_y);

	this->m_spaceship_speed = 125.0f;

	if(this->m_spaceship_turbo_mode && this->m_forward != 0.0f)
	{
		this->m_spaceship_speed = 1000.0f;
		this->m_distortion_level = lerp(this->m_distortion_level, 0.6f, this->m_delta_time * 2.0f);
	}
	else
	{
		this->m_distortion_level = lerp(this->m_distortion_level, 1.0f, this->m_delta_time * 2.0f);
	}

	this->m_spaceship_pos = this->m_spaceship_pos +
		world_model.getDirectionZ() * this->m_forward * this->m_spaceship_speed * this->m_delta_time +
		world_model.getDirectionX() * this->m_rightward * this->m_spaceship_speed * this->m_delta_time +
		world_model.getDirectionY() * this->m_upward * this->m_spaceship_speed * this->m_delta_time;

	this->m_current_spaceship_pos = Vector3::lerp(this->m_current_spaceship_pos, this->m_spaceship_pos, 3.0f * this->m_delta_time);
}

void PostProcessingDemo::updateViewportProjection()
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	this->m_proj_cam.setPerspectiveFovPH(1.57f, ((float)width / (float)height), zNear, zFar);
}

void PostProcessingDemo::drawMesh(const MeshPtr &mesh, const std::vector<MaterialPtr> &list_materials)
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