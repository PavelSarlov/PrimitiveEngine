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