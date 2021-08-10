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
		{-0.5f, -0.5f, 0.0f},
		{-0.5f, 0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f},
		{0.5f, 0.5f, 0.0f},

		/*{0.5f, 0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f},
		{-0.5f, -0.5f, 0.0f}*/
	};
	UINT size_list = ARRAYSIZE(list);

	this->m_vb = PrimitiveEngine::get()->createVertexBuffer();

	PrimitiveEngine::get()->createShaders();

	void *shader_byte_code = nullptr;
	size_t size_shader = 0;
	PrimitiveEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	this->m_vs = PrimitiveEngine::get()->createVertexShader(shader_byte_code, size_shader);

	this->m_vb->load(list, sizeof(Vertex), size_list, shader_byte_code, size_shader);

	PrimitiveEngine::get()->releaseCompiledShader();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	// clear render target
	PrimitiveEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	// set viewport of render target in which we will draw
	RECT rect = this->getClientWindowRect();
	PrimitiveEngine::get()->getImmediateDeviceContext()->setViewPortSize(rect.right - rect.left, rect.bottom - rect.top);

	// set default shader in the graphics pipeline
	PrimitiveEngine::get()->setShaders();
	PrimitiveEngine::get()->getImmediateDeviceContext()->setVertexShader(this->m_vs);

	// set the list of vertices
	PrimitiveEngine::get()->getImmediateDeviceContext()->setVertexBuffer(this->m_vb);

	// draw the vertices
	//PrimitiveEngine::get()->getImmediateDeviceContext()->drawTriangleList(this->m_vb->getSizeVertexList(), 0);
	PrimitiveEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(this->m_vb->getSizeVertexList(), 0);

	this->m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	this->m_vb->release();
	this->m_swap_chain->release();
	PrimitiveEngine::get()->release();
}