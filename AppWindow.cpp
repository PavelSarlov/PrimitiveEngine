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
		{-0.5f,-0.5f,0.0f,		-0.32f,-0.11f,0.0f,		0,0,0,	0,1,0},
		{-0.5f, 0.5f,0.0f,		-0.11f, 0.78f,0.0f,		1,1,0,	0,1,1},
		{ 0.5f,-0.5f,0.0f,		 0.75f,-0.73f,0.0f,		0,0,1,	1,0,0},
		{ 0.5f, 0.5f,0.0f,		 0.88f, 0.77f,0.0f,		1,1,1,	0,0,1},
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
	Constant cc = {};
	cc.m_time = ::GetTickCount64();
	this->m_cb->update(PrimitiveEngine::get()->getImmediateDeviceContext(), &cc);

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