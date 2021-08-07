#include "AppWindow.h"

AppWindow::AppWindow()
{}

AppWindow::~AppWindow()
{}

void AppWindow::OnCreate()
{
	Window::OnCreate();
	PrimitiveEngine::Get()->Init();
	this->m_swap_chain = PrimitiveEngine::Get()->CreateSwapChain();

	RECT rect = this->GetClientWindowRect();
	this->m_swap_chain->Init(this->m_hwnd, rect.right - rect.left, rect.bottom - rect.top);

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

	this->m_vb = PrimitiveEngine::Get()->CreateVertexBuffer();

	PrimitiveEngine::Get()->CreateShaders();

	void *shader_byte_code = nullptr;
	size_t size_shader = 0;
	PrimitiveEngine::Get()->CompileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	this->m_vs = PrimitiveEngine::Get()->CreateVertexShader(shader_byte_code, size_shader);

	this->m_vb->Load(list, sizeof(Vertex), size_list, shader_byte_code, size_shader);

	PrimitiveEngine::Get()->ReleaseCompiledShader();
}

void AppWindow::OnUpdate()
{
	Window::OnUpdate();

	// clear render target
	PrimitiveEngine::Get()->GetImmediateDeviceContext()->ClearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);

	// set viewport of render target in which we will draw
	RECT rect = this->GetClientWindowRect();
	PrimitiveEngine::Get()->GetImmediateDeviceContext()->SetViewPortSize(rect.right - rect.left, rect.bottom - rect.top);

	// set default shader in the graphics pipeline
	PrimitiveEngine::Get()->SetShaders();
	PrimitiveEngine::Get()->GetImmediateDeviceContext()->SetVertexShader(this->m_vs);

	// set the list of vertices
	PrimitiveEngine::Get()->GetImmediateDeviceContext()->SetVertexBuffer(this->m_vb);

	// draw the vertices
	//PrimitiveEngine::Get()->GetImmediateDeviceContext()->DrawTriangleList(this->m_vb->GetSizeVertexList(), 0);
	PrimitiveEngine::Get()->GetImmediateDeviceContext()->DrawTriangleStrip(this->m_vb->GetSizeVertexList(), 0);

	this->m_swap_chain->Present(true);
}

void AppWindow::OnDestroy()
{
	Window::OnDestroy();
	this->m_vb->Release();
	this->m_swap_chain->Release();
	PrimitiveEngine::Get()->Release();
}