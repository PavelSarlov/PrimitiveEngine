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

#include <Mesh.h>
#include <GraphicsEngine.h>
#include <Geometry.h>




Mesh::Mesh(const VertexBufferPtr &vertex_buffer, const IndexBufferPtr &index_buffer, UINT material_id) :
	m_vertex_buffer(vertex_buffer), m_index_buffer(index_buffer), m_material_id(material_id)
{}

Mesh::~Mesh()
{}

const VertexBufferPtr &Mesh::getVertexBuffer()
{
	return this->m_vertex_buffer;
}

const IndexBufferPtr &Mesh::getIndexBuffer()
{
	return this->m_index_buffer;
}

const BoundingBox &Mesh::getBoundingBox()
{
	return this->m_bounding_box;
}

const BoundingSphere &Mesh::getBoundingSphere()
{
	return this->m_bounding_sphere;
}

UINT Mesh::getMaterialId()
{
	return this->m_material_id;
}
