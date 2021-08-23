#include "Texture.h"
#include "GraphicsEngine.h"

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

Texture::~Texture()
{
	if(this->m_shader_res_view) this->m_shader_res_view->Release();
	if(this->m_sampler_state) this->m_sampler_state->Release();
	if(this->m_texture) this->m_texture->Release();
}