#include "AppWindow.h"

AppWindow::AppWindow()
{}

AppWindow::~AppWindow()
{}

void AppWindow::onCreate()
{
	Window::onCreate();
	PrimitiveEngine::get()->init();
	this->m_swap_chain = PrimitiveEngine::get()->createSwapChain();

	RECT rect = this->getClientWindowRect();
	this->m_swap_chain->init(this->m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

	Vertex list[] =
	{
		{Vector3(-0.5f,-0.5f,0.0f),		Vector3(-0.32f,-0.11f,0.0f),	Vector3(0,0,0),		Vector3(0,1,0)},
		{Vector3(-0.5f, 0.5f,0.0f),		Vector3(-0.11f, 0.78f,0.0f),	Vector3(1,1,0),		Vector3(0,1,1)},
		{Vector3(0.5f,-0.5f,0.0f),		Vector3(0.75f,-0.73f,0.0f),		Vector3(0,0,1),		Vector3(1,0,0)},
		{Vector3(0.5f, 0.5f,0.0f),		Vector3(0.88f, 0.77f,0.0f),		Vector3(1,1,1),		Vector3(0,0,1)},
	};
	UINT size_list = ARRAYSIZE(list);

	this->m_vb = PrimitiveEngine::get()->createVertexBuffer();

	void *shader_byte_code = nullptr;
	size_t size_shader = 0;

	// create vertex shader
	PrimitiveEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	this->m_vs = PrimitiveEngine::get()->createVertexShader(shader_byte_code, size_shader);
	this->m_vb->load(list, sizeof(Vertex), size_list, shader_byte_code, size_shader);
	PrimitiveEngine::get()->releaseCompiledShader();

	// create pixel shader
	PrimitiveEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	this->m_ps = PrimitiveEngine::get()->createPixelShader(shader_byte_code, size_shader);
	PrimitiveEngine::get()->releaseCompiledShader();

	// create constant buffer
	Constant cc = {};
	cc.m_time = 0;
	this->m_cb = PrimitiveEngine::get()->createConstantBuffer();
	this->m_cb->load(&cc, sizeof(Constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	// clear render target
	PrimitiveEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	// set viewport of render target in which we will draw
	RECT rect = this->getClientWindowRect();
	PrimitiveEngine::get()->getImmediateDeviceContext()->setViewPortSize(rect.right - rect.left, rect.bottom - rect.top);

	// update constant buffer
	this->updateQuadPosition();

	// bind the constant buffer to the graphics pipeline for each shader
	PrimitiveEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_vs, this->m_cb);
	PrimitiveEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_ps, this->m_cb);

	// set default shader in the graphics pipeline
	PrimitiveEngine::get()->getImmediateDeviceContext()->setVertexShader(this->m_vs);
	PrimitiveEngine::get()->getImmediateDeviceContext()->setPixelShader(this->m_ps);

	// set the list of vertices
	PrimitiveEngine::get()->getImmediateDeviceContext()->setVertexBuffer(this->m_vb);

	// draw the figure
	//PrimitiveEngine::get()->getImmediateDeviceContext()->drawTriangleList(this->m_vb->getSizeVertexList(), 0);
	PrimitiveEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(this->m_vb->getSizeVertexList(), 0);
	this->m_swap_chain->present(true);


	// timing
	this->m_old_delta = this->m_new_delta;
	this->m_new_delta = ::GetTickCount();
	this->m_delta_time = (this->m_old_delta) ? ((this->m_new_delta - this->m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	this->m_vb->release();
	this->m_swap_chain->release();
	this->m_vs->release();
	this->m_ps->release();
	this->m_cb->release();
	PrimitiveEngine::get()->release();
}

void AppWindow::updateQuadPosition()
{
	Constant cc = {};
	cc.m_time = ::GetTickCount();

	this->m_delta_pos += this->m_delta_time / 10.0f;

	if(this->m_delta_pos > 1.0f)
	{
		this->m_delta_pos = 0.0f;
	}

	Matrix4x4 temp;

	// cc.m_world = Matrix4x4::translationMatrix(Vector3::lerp(Vector3(-2, -2, 0), Vector3(2, 2, 0), this->m_delta_pos));

	this->m_delta_scale += this->m_delta_time / 0.15f;

	cc.m_world = Matrix4x4::scaleMatrix(Vector3::lerp(Vector3(0.5f, 0.5f, 0), Vector3(1.0f, 1.0f, 0), (sin(this->m_delta_scale) + 1.0f) / 2.0f));

	temp = Matrix4x4::translationMatrix(Vector3::lerp(Vector3(-1.5, -1.5, 0), Vector3(1.5f, 1.5f, 0), this->m_delta_pos));

	cc.m_world *= temp;



	cc.m_view = Matrix4x4::identityMatrix();
	cc.m_proj = Matrix4x4::orthogonalProjMatrix
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	);



	this->m_cb->update(PrimitiveEngine::get()->getImmediateDeviceContext(), &cc);
}
