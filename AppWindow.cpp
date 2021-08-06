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

	Triangle tr =
	{
		{
		{{-0.5f, -0.5f, 0.0f}},
		{{0.0f, 0.5f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}}
		}
	};
	UINT size_list = ARRAYSIZE(tr.p);
	
	this->m_vb = PrimitiveEngine::Get()->CreateVertexBuffer();

	PrimitiveEngine::Get()->CreateShaders();
	void *shader_byte_code = nullptr;
	UINT size_shader = 0;
	PrimitiveEngine::Get()->GetShaderBufferAndSize(&shader_byte_code, &size_shader);

	this->m_vb->Load(tr.p, sizeof(Vertex), size_list, shader_byte_code, size_shader);
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

	// set the list if vertices
	PrimitiveEngine::Get()->GetImmediateDeviceContext()->SetVertexBuffer(this->m_vb);

	// draw the vertices
	PrimitiveEngine::Get()->GetImmediateDeviceContext()->DrawTriangleList(this->m_vb->GetSizeVertexList(), 0);
	this->m_swap_chain->Present(true);
}

void AppWindow::OnDestroy()
{
	Window::OnDestroy();
	this->m_vb->Release();
	this->m_swap_chain->Release();
	PrimitiveEngine::Get()->Release();
}