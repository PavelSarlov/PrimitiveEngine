#include "Mesh.h"

#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif
#include <tiny_obj_loader.h>

Mesh::Mesh(const wchar_t *full_path) : Resource(full_path)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	int size_needed = ::WideCharToMultiByte(CP_UTF8, 0, &full_path[0], wcslen(full_path), NULL, 0, NULL, NULL);
	std::string inputFile(size_needed, 0);
	::WideCharToMultiByte(CP_UTF8, 0, &full_path[0], wcslen(full_path), &inputFile[0], size_needed, NULL, NULL);

	if(!tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputFile.c_str()) ||
		!err.empty() ||
		shapes.size() > 1)
	{
		throw std::exception("Mesh failed to load");
	}

	std::vector<VertexMesh> list_vertices;
	std::vector<UINT> list_indices;

	for(size_t s = 0; s < shapes.size(); s++)
	{
		size_t index_offset = 0;
		list_vertices.reserve(shapes[s].mesh.indices.size());
		list_indices.reserve(shapes[s].mesh.indices.size());

		for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			UCHAR num_face_verts = shapes[s].mesh.num_face_vertices[f];

			for(UCHAR v = 0; v < num_face_verts; v++)
			{
				tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attribs.vertices[(ULONGLONG)index.vertex_index * 3];
				tinyobj::real_t vy = attribs.vertices[(ULONGLONG)index.vertex_index * 3 + 1];
				tinyobj::real_t vz = attribs.vertices[(ULONGLONG)index.vertex_index * 3 + 2];

				tinyobj::real_t tx = attribs.texcoords[(ULONGLONG)index.texcoord_index * 2];
				tinyobj::real_t ty = attribs.texcoords[(ULONGLONG)index.texcoord_index * 2 + 1];

				tinyobj::real_t nx = attribs.normals[(ULONGLONG)index.normal_index * 3];
				tinyobj::real_t ny = attribs.normals[(ULONGLONG)index.normal_index * 3 + 1];
				tinyobj::real_t nz = attribs.normals[(ULONGLONG)index.normal_index * 3 + 2];

				VertexMesh vertex(Vector3(vx, vy, vz), Vector2(tx, ty), Vector3(nx, ny, nz));
				list_vertices.push_back(vertex);

				list_indices.push_back((UINT)index_offset + v);
			}

			index_offset += num_face_verts;
		}
	}

	void *shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVertexLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	this->m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&list_vertices[0], sizeof(VertexMesh), list_vertices.size(), shader_byte_code, size_shader);

	this->m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0], list_indices.size());
}

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
