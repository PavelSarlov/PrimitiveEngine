#pragma once
#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "Prerequisites.h"
#include "RenderSystem.h"

#include <d3d11.h>
#include <exception>

class DeviceContext;

class VertexBuffer
{
public:
	VertexBuffer(void *list_vertices, UINT size_vertex, UINT size_list, void *shader_byte_code, size_t size_byte_shader, RenderSystem *system);
	~VertexBuffer();

public:
	UINT getSizeVertexList();

private:
	UINT m_size_vertex = 0;
	UINT m_size_list = 0;

private:
	ID3D11Buffer *m_buffer = nullptr;
	ID3D11InputLayout *m_layout = nullptr;
	RenderSystem *m_system = nullptr;

private:
	friend class DeviceContext;
};

#endif // !VERTEXBUFFER_H