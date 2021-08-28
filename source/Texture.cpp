// MIT License
// 
// C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)
// 
// Copyright (c) 2019-2021, PardCode
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <Texture.h>
#include <GraphicsEngine.h>

#include <DirectXTex.h>

Texture::Texture(const wchar_t *full_path) : Resource(full_path)
{
	DirectX::ScratchImage image_data;

	HRESULT hrs = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);
	if(FAILED(hrs))
	{
		throw std::exception("Texture loading from file failed");
	}

	hrs = DirectX::CreateTexture(GraphicsEngine::get()->getRenderSystem()->m_d3d_device, image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &this->m_texture);
	if(FAILED(hrs))
	{
		throw std::exception("Texture creation failed");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = image_data.GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = (UINT)image_data.GetMetadata().mipLevels;
	desc.Texture2D.MostDetailedMip = 0;

	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = (FLOAT)image_data.GetMetadata().mipLevels;

	hrs = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateSamplerState(&sampler_desc, &this->m_sampler_state);
	if(FAILED(hrs))
	{
		throw std::exception("SamplerState creation failed");
	}

	hrs = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(this->m_texture, &desc, &this->m_shader_res_view);
	if(FAILED(hrs))
	{
		throw std::exception("ShaderResourceView creation failed");
	}
}

Texture::Texture(const Rect &size, Texture::Type type) : Resource(L"")
{
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = size.m_width;
	tex_desc.Height = size.m_height;

	switch(type)
	{
	case NORMAL:
	{
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		break;
	}
	case RENDER_TARGET:
	{
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		break;
	}
	case DEPTH_STENCIL:
	{
		tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		break;
	}
	}

	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	HRESULT hrs = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateTexture2D(&tex_desc, nullptr, (ID3D11Texture2D **)&this->m_texture);
	if(FAILED(hrs))
	{
		throw std::exception("Texture creation failed");
	}

	if(type == NORMAL || type == RENDER_TARGET)
	{
		hrs = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(this->m_texture, nullptr, &this->m_shader_res_view);
		if(FAILED(hrs))
		{
			throw std::exception("Texture SRV creation failed");
		}
	}

	if(type == RENDER_TARGET)
	{
		hrs = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateRenderTargetView(this->m_texture, nullptr, &this->m_render_target_view);
		if(FAILED(hrs))
		{
			throw std::exception("Texture RTV creation failed");
		}
	}
	else if(type == DEPTH_STENCIL)
	{
		hrs = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateDepthStencilView(this->m_texture, NULL, &this->m_depth_stencil_view);
		if(FAILED(hrs))
		{
			throw std::exception("Texture DSV creation failed");
		}
	}

	this->m_size = size;
	this->m_type = type;
}

Texture::~Texture()
{
	if(this->m_render_target_view) this->m_render_target_view->Release();
	if(this->m_depth_stencil_view) this->m_depth_stencil_view->Release();
	if(this->m_shader_res_view) this->m_shader_res_view->Release();
	if(this->m_sampler_state) this->m_sampler_state->Release();
	if(this->m_texture) this->m_texture->Release();
}

Rect Texture::getSize()
{
	return this->m_size;
}

Texture::Type Texture::getType()
{
	return this->m_type;
}
