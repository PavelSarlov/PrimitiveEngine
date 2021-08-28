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
#ifndef MESH_H
#define MESH_H

#include <Resource.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <Geometry.h>
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
	const BoundingBox &getBoundingBox();
	const BoundingSphere &getBoundingSphere();

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
	BoundingBox m_bounding_box;
	BoundingSphere m_bounding_sphere;

private:
	friend class DeviceContext;
};

#endif