#pragma once

#ifndef PIXELSHADER_H
#define PIXELSHADER_H

#include <d3d11.h>
#include <exception>

#include "RenderSystem.h"
#include "Prerequisites.h"

class PixelShader
{
public:
	PixelShader(const void *shader_byte_code, size_t byte_code_size, RenderSystem *system);
	~PixelShader();

private:
	ID3D11PixelShader *m_ps = nullptr;
	RenderSystem *m_system = nullptr;

private:
	friend class RenderSystem;
	friend class DeviceContext;
};

#endif // !PIXELSHADER_H