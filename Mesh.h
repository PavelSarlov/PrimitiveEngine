#pragma once
#ifndef MESH_H
#define MESH_H

#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "GraphicsEngine.h"
#include "Geometry.h"

class Mesh : public Resource
{
public:
	Mesh(const wchar_t *full_path);
	~Mesh();

public:
	const VertexBufferPtr &getVertexBuffer();
	const IndexBufferPtr &getIndexBuffer();

private:
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;

private:
	friend class DeviceContext;
};

#endif