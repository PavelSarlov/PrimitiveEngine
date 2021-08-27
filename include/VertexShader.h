#pragma once

#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include "Prerequisites.h"

#include <d3d11.h>

class VertexShader
{
public:
	VertexShader(const void *shader_byte_code, size_t byte_code_size, RenderSystem *system);
	~VertexShader();

private:
	ID3D11VertexShader *m_vs = nullptr;
	RenderSystem *m_system = nullptr;

private:
	friend class RenderSystem;
	friend class DeviceContext;
};


#endif // VERTEXSHADER_H