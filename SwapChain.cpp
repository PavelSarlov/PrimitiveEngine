#include "SwapChain.h"
#include "RenderSystem.h"

#include <exception>

SwapChain::SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem *system) : m_system(system)
{
	ID3D11Device *device = this->m_system->m_d3d_device;

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
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Windowed = TRUE;

	// Create swapchain for the window indicated by HWND parameter
	HRESULT hrs = this->m_system->m_dxgi_factory->CreateSwapChain(device, &desc, &this->m_swap_chain);

	if(FAILED(hrs))
	{
		throw std::exception("SwapChain creation failed");;
	}

	this->reloadBuffers(width, height);
}

SwapChain::~SwapChain()
{
	if(this->m_rtv) this->m_rtv->Release();
	if(this->m_dsv) this->m_dsv->Release();
	if(this->m_swap_chain) this->m_swap_chain->Release();
}

bool SwapChain::present(bool vsync)
{
	this->m_swap_chain->Present(vsync, NULL);
	return true;
}

void SwapChain::resize(UINT width, UINT height)
{
	if(this->m_rtv) this->m_rtv->Release();
	if(this->m_dsv) this->m_dsv->Release();

	this->m_swap_chain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, NULL);
	this->reloadBuffers(width, height);
}

void SwapChain::setFullScreen(bool fullscreen, UINT width, UINT height)
{
	this->resize(width, height);
	this->m_swap_chain->SetFullscreenState(fullscreen, nullptr);
}

void SwapChain::reloadBuffers(UINT width, UINT height)
{
	ID3D11Device *device = this->m_system->m_d3d_device;

	// get the back buffer color and create its render target view
	ID3D11Texture2D *buffer = NULL;
	HRESULT hrs = this->m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&buffer);

	if(FAILED(hrs))
	{
		throw std::exception("SwapChain back buffer creation failed");;
	}

	hrs = device->CreateRenderTargetView(buffer, NULL, &this->m_rtv);
	buffer->Release();

	if(FAILED(hrs))
	{
		throw std::exception("SwapChain RTV creation failed");
	}

	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	hrs = device->CreateTexture2D(&tex_desc, nullptr, &buffer);

	if(FAILED(hrs))
	{
		throw std::exception("SwapChain texture creation failed");
	}

	hrs = device->CreateDepthStencilView(buffer, NULL, &this->m_dsv);
	buffer->Release();

	if(FAILED(hrs))
	{
		throw std::exception("SwapChain DSV creation failed");
	}
}
