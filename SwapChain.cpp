#include "SwapChain.h"

SwapChain::SwapChain()
{}

SwapChain::~SwapChain()
{}

bool SwapChain::init(HWND hwnd, UINT width, UINT height)
{
	ID3D11Device *device = PrimitiveEngine::get()->m_d3d_device;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;

	// Create swapchain for the window indicated by HWND parameter
	HRESULT hrs = PrimitiveEngine::get()->m_dxgi_factory->CreateSwapChain(device, &desc, &this->m_swap_chain);

	if(FAILED(hrs))
	{
		return false;
	}

	// get the back buffer color and create its render target view
	ID3D11Texture2D *buffer = NULL;
	hrs = this->m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&buffer);

	if(FAILED(hrs))
	{
		return false;
	}

	hrs = device->CreateRenderTargetView(buffer, NULL, &this->m_rtv);
	buffer->Release();

	if(FAILED(hrs))
	{
		return false;
	}

	return true;
}

bool SwapChain::release()
{
	if(this->m_swap_chain) this->m_swap_chain->Release();
	if(this->m_rtv) this->m_rtv->Release();
	delete this;
	return true;
}

bool SwapChain::present(bool vsync)
{
	this->m_swap_chain->Present(vsync, NULL);

	return true;
}
