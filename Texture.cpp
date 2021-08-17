#include "Texture.h"

Texture::Texture(const wchar_t *full_path) : Resource(full_path)
{
	DirectX::ScratchImage image_data;
	HRESULT hrs = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_NONE, nullptr, image_data);

	if(FAILED(hrs))
	{
		throw std::exception("Texture creation failed");
	}

	hrs = DirectX::CreateTexture(GraphicsEngine::get()->getRenderSystem()->m_d3d_device, image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &this->m_texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = image_data.GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
	desc.Texture2D.MostDetailedMip = 0;

	GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(this->m_texture, &desc, &this->m_shader_res_view);
}

Texture::~Texture()
{
	if(this->m_shader_res_view) this->m_shader_res_view->Release();
	if(this->m_texture) this->m_texture->Release();
}