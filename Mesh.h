#pragma once
#ifndef MESH_H
#define MESH_H

#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Geometry.h"

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
	Mesh(VertexMesh *vertex_list_data, unsigned int vertex_list_size,
		unsigned int *index_list_data, unsigned int index_list_size,
		MaterialSlot *material_slot_list, unsigned int material_slot_list_size);
	~Mesh();

public:
	const VertexBufferPtr &getVertexBuffer();
	const IndexBufferPtr &getIndexBuffer();

public:
	const MaterialSlot &getMaterialSlot(size_t slot);
	size_t getNumMaterialSlots();

public:
	void computeTangents(
		const Vector3 &v0, const Vector3 &v1, const Vector3 &v2,
		const Vector2 &t0, const Vector2 &t1, const Vector2 &t2,
		Vector3 &tangent, Vector3 &binormal
	);

private:
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;
	std::vector<MaterialSlot> m_material_slots;

private:
	friend class DeviceContext;
};

#endif