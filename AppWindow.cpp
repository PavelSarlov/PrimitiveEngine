#include "AppWindow.h"

AppWindow::AppWindow() : Window()
{}

AppWindow::~AppWindow()
{}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	this->m_wood_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\brick.png");
	this->m_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Meshes\\teapot.obj");

	// create swap chain
	RECT rect = this->getClientWindowRect();
	this->m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	this->m_world_cam = Matrix4x4::translationMatrix(0, 0, -2);

	// shaders data
	void *shader_byte_code = nullptr;
	size_t size_shader = 0;

	// create vertex shader
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	this->m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	// create pixel shader
	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	this->m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	// create constant buffer
	Constant cc = {};
	cc.m_time = 0;
	this->m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(Constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	// clear render target
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	// set viewport of render target in which we will draw
	RECT rect = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewPortSize(rect.right - rect.left, rect.bottom - rect.top);

	// update constant buffer
	this->update();

	// bind the constant buffer to the graphics pipeline for each shader
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(this->m_vs, this->m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(this->m_ps, this->m_cb);

	// set default shader in the graphics pipeline
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(this->m_vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(this->m_ps);

	// set wood texture
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(this->m_ps, this->m_wood_tex);

	// set the list of vertices
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(this->m_mesh->getVertexBuffer());
	// set the list of indices
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(this->m_mesh->getIndexBuffer());

	// draw the object
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(this->m_mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
	this->m_swap_chain->present(true);

	// timing
	this->m_old_delta = this->m_new_delta;
	this->m_new_delta = ::GetTickCount64();
	this->m_delta_time = (this->m_new_delta - this->m_old_delta) / 1000.0f;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::update()
{
	Constant cc = {};
	cc.m_time = ::GetTickCount64();

	this->m_delta_pos += this->m_delta_time / 10.0f;

	if(this->m_delta_pos > 1.0f)
	{
		this->m_delta_pos = 0.0f;
	}

	Matrix4x4 temp;

	// cc.m_world = Matrix4x4::translationMatrix(Vector3::lerp(Vector3(-2, -2, 0), Vector3(2, 2, 0), this->m_delta_pos));

	this->m_delta_scale += this->m_delta_time / 0.55f;

	/*cc.m_world = Matrix4x4::scaleMatrix(Vector3::lerp(Vector3(0.5f, 0.5f, 0), Vector3(1.0f, 1.0f, 0), (sin(this->m_delta_scale) + 1.0f) / 2.0f));

	temp = Matrix4x4::translationMatrix(Vector3::lerp(Vector3(-1.5, -1.5, 0), Vector3(1.5f, 1.5f, 0), this->m_delta_pos));

	cc.m_world *= temp;*/

	/*cc.m_world = Matrix4x4::scaleMatrix(this->m_scale_cube, this->m_scale_cube, this->m_scale_cube);

	temp = Matrix4x4::rotationZ(0.0f);
	cc.m_world *= temp;

	temp = Matrix4x4::rotationY(this->m_rot_y);
	cc.m_world *= temp;

	temp = Matrix4x4::rotationX(this->m_rot_x);
	cc.m_world *= temp;*/

	cc.m_world = Matrix4x4::identityMatrix();
	Matrix4x4 world_cam = Matrix4x4::identityMatrix();

	world_cam *= Matrix4x4::rotationX(m_rot_x);
	world_cam *= Matrix4x4::rotationY(m_rot_y);

	Vector3 new_pos = this->m_world_cam.getTranslation() + world_cam.getDirectionZ() * this->m_forward * 0.03f + world_cam.getDirectionX() * this->m_rightward * 0.03f;

	world_cam.setTranslation(new_pos);
	this->m_world_cam = world_cam;
	world_cam = world_cam.inversedMatrix();

	cc.m_view = world_cam;
	//cc.m_proj = Matrix4x4::orthogonalProjMatrix
	//(
	//	(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
	//	(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
	//	-4.0f,
	//	4.0f
	//);

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	cc.m_proj.setPerspectiveFovPH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);


	this->m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onKeyDown(USHORT key)
{
	switch(key)
	{
	case 'W':
	{
		//m_rot_x += 3.14f * this->m_delta_time;
		this->m_forward = 1.0f;

		break;
	}
	case 'S':
	{
		// m_rot_x -= 3.14f * this->m_delta_time;
		this->m_forward = -1.0f;
		break;
	}
	case 'A':
	{
		// m_rot_y += 3.14f * this->m_delta_time;
		this->m_rightward = -1.0f;

		break;
	}
	case 'D':
	{
		// m_rot_y -= 3.14f * this->m_delta_time;
		this->m_rightward = 1.0f;

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
	}
}

void AppWindow::onMouseMove(const Point &mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	this->m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * this->m_delta_time * 0.1f;
	this->m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * this->m_delta_time * 0.1f;

	InputSystem::get()->setCursorPos(Point(width / 2.0f, height / 2.0f));
}

void AppWindow::onLeftMouseDown(const Point &mouse_pos)
{
	this->m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point &mouse_pos)
{
	this->m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point &mouse_pos)
{
	this->m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point &mouse_pos)
{
	this->m_scale_cube = 1.0f;
}
