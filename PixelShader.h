#pragma once

#ifndef PIXELSHADER_H
#define PIXELSHADER_H

#include <d3d11.h>

#include "PrimitiveEngine.h"

class PrimitiveEngine;
class DeviceContext;

class PixelShader
{
public:
	PixelShader();
	~PixelShader();

public:
	void release();

private:
	bool init(const void *shader_byte_code, size_t byte_code_size);

private:
	ID3D11PixelShader *m_ps = nullptr;

private:
	friend class PrimitiveEngine;
	friend class DeviceContext;
};

#endif // !PIXELSHADER_H