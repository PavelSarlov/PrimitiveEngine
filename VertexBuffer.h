#pragma once

#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "PrimitiveEngine.h"

#include <d3d11.h>

class DeviceContext;

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

public:
	bool Load(void *list_vertices, UINT size_vertex, UINT size_list, void *shader_byte_code, UINT size_byte_shader);
	bool Release();
	UINT GetSizeVertexList();

private:
	UINT m_size_vertex = 0;
	UINT m_size_list = 0;

private:
	ID3D11Buffer *m_buffer = nullptr;
	ID3D11InputLayout *m_layout = nullptr;

private:
	friend class DeviceContext;
};

#endif // !VERTEXBUFFER_H