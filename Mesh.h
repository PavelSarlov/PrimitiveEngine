#pragma once
#ifndef MESH_H
#define MESH_H

#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>

struct MaterialSlot
{
	size_t start_index = 0;
	size_t num_indices = 0;
	size_t material_id = 0;
};

class Mesh : public Resource
{
public:
	Mesh(const wchar_t *full_path);
	~Mesh();

public:
	const VertexBufferPtr &getVertexBuffer();
	const IndexBufferPtr &getIndexBuffer();

public:
	const MaterialSlot &getMaterialSlot(size_t slot);
	size_t getNumMaterialSlots();

private:
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;
	std::vector<MaterialSlot> m_material_slots;

private:
	friend class DeviceContext;
};

#endif