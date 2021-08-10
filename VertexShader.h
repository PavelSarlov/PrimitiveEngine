#pragma once

#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include <d3d11.h>

#include "PrimitiveEngine.h"

class PrimitiveEngine;
class DeviceContext;

class VertexShader
{
public :
	VertexShader();
	~VertexShader();

public:
	void release();

private:
	bool init(const void *shader_byte_code, size_t byte_code_size);

private:
	ID3D11VertexShader *m_vs = nullptr;

private:
	friend class PrimitiveEngine;
	friend class DeviceContext;
};


#endif // VERTEXSHADER_H