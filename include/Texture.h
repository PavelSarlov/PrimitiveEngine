#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "Resource.h"
#include "Geometry.h"

#include <d3d11.h>

class Texture : public Resource
{
public:
	enum Type
	{
		NORMAL = 0,
		RENDER_TARGET,
		DEPTH_STENCIL
	};

public:
	Texture(const wchar_t *full_path);
	Texture(const Rect &size, Texture::Type type);
	~Texture();

public:
	Rect getSize();
	Texture::Type getType();

private:
	ID3D11Resource *m_texture = nullptr;
	ID3D11ShaderResourceView *m_shader_res_view = nullptr;
	ID3D11RenderTargetView *m_render_target_view = nullptr;
	ID3D11DepthStencilView *m_depth_stencil_view = nullptr;
	ID3D11SamplerState *m_sampler_state = nullptr;

private:
	Texture::Type m_type = Texture::Type::NORMAL;
	Rect m_size;

private:
	friend class DeviceContext;
};

#endif // !TEXTURE_H