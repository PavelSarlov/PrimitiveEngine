#include "Mesh.h"
#include "GraphicsEngine.h"
#include "Geometry.h"

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

	int size_needed = ::WideCharToMultiByte(CP_UTF8, 0, &full_path[0], (int)wcslen(full_path), NULL, 0, NULL, NULL);
	std::string inputFile(size_needed, 0);
	::WideCharToMultiByte(CP_UTF8, 0, &full_path[0], (int)wcslen(full_path), &inputFile[0], size_needed, NULL, NULL);

	std::string mtldir = inputFile.substr(0, inputFile.find_last_of("\\/"));

	if(!tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputFile.c_str(), mtldir.c_str()) ||
		!err.empty())
	{
		throw std::exception("Mesh failed to load");
	}

	std::vector<VertexMesh> list_vertices;
	std::vector<UINT> list_indices;

	size_t size_vertex_index_lists = 0;

	for(size_t s = 0; s < shapes.size(); s++)
	{
		size_vertex_index_lists += shapes[s].mesh.indices.size();
	}

	list_vertices.reserve(size_vertex_index_lists);
	list_indices.reserve(size_vertex_index_lists);

	this->m_material_slots.resize(materials.size());

	size_t index_global_offset = 0;

	for(size_t m = 0; m < materials.size(); m++)
	{
		this->m_material_slots[m].start_index = index_global_offset;
		this->m_material_slots[m].material_id = m;

		for(size_t s = 0; s < shapes.size(); s++)
		{
			size_t index_offset = 0;

			for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				UCHAR num_face_verts = shapes[s].mesh.num_face_vertices[f];

				if(shapes[s].mesh.material_ids[f] != m)
				{
					index_offset += num_face_verts;
					continue;
				}

				Vector3 vertices_face[3];
				Vector2 texcoords_face[3];

				for(UCHAR v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[(ULONGLONG)index.vertex_index * 3];
					tinyobj::real_t vy = attribs.vertices[(ULONGLONG)index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[(ULONGLONG)index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;

					if(attribs.texcoords.size())
					{
						tx = attribs.texcoords[(ULONGLONG)index.texcoord_index * 2];
						ty = attribs.texcoords[(ULONGLONG)index.texcoord_index * 2 + 1];
					}

					vertices_face[v] = Vector3(vx, vy, vz);
					texcoords_face[v] = Vector2(tx, ty);
				}

				Vector3 tangent, binormal;
				computeTangents(
					vertices_face[0], vertices_face[1], vertices_face[2],
					texcoords_face[0], texcoords_face[1], texcoords_face[2],
					tangent, binormal
				);

				for(UCHAR v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[(ULONGLONG)index.vertex_index * 3];
					tinyobj::real_t vy = attribs.vertices[(ULONGLONG)index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[(ULONGLONG)index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;

					if(attribs.texcoords.size())
					{
						tx = attribs.texcoords[(ULONGLONG)index.texcoord_index * 2];
						ty = attribs.texcoords[(ULONGLONG)index.texcoord_index * 2 + 1];
					}

					tinyobj::real_t nx = 0;
					tinyobj::real_t ny = 0;
					tinyobj::real_t nz = 0;

					if(attribs.normals.size())
					{
						nx = attribs.normals[(ULONGLONG)index.normal_index * 3];
						ny = attribs.normals[(ULONGLONG)index.normal_index * 3 + 1];
						nz = attribs.normals[(ULONGLONG)index.normal_index * 3 + 2];
					}

					Vector3 v_tangent, v_binormal;
					v_binormal = Vector3::crossProduct(Vector3(nx, ny, nz), tangent);
					v_tangent = Vector3::crossProduct(binormal, Vector3(nx, ny, nz));

					VertexMesh vertex(Vector3(vx, vy, vz), Vector2(tx, ty), Vector3(nx, ny, nz), v_tangent, v_binormal);
					list_vertices.push_back(vertex);

					list_indices.push_back((UINT)index_global_offset + v);
				}

				index_offset += num_face_verts;
				index_global_offset += num_face_verts;
			}
		}

		this->m_material_slots[m].num_indices = index_global_offset - this->m_material_slots[m].start_index;
	}

	void *shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVertexLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	this->m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&list_vertices[0], sizeof(VertexMesh), (UINT)list_vertices.size(), shader_byte_code, size_shader);

	this->m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());
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

const MaterialSlot &Mesh::getMaterialSlot(size_t slot)
{
	return (slot >= this->m_material_slots.size()) ? MaterialSlot() : this->m_material_slots[slot];
}

size_t Mesh::getNumMaterialSlots()
{
	return this->m_material_slots.size();
}

void Mesh::computeTangents(
	const Vector3 &v0, const Vector3 &v1, const Vector3 &v2,
	const Vector2 &t0, const Vector2 &t1, const Vector2 &t2,
	Vector3 &tangent, Vector3 &binormal)
{
	Vector3 deltaPos1 = v1 - v0;
	Vector3 deltaPos2 = v2 - v0;

	Vector2 deltaUV1 = t1 - t0;
	Vector2 deltaUV2 = t2 - t0;

	float r = 1.0f / (deltaUV1.m_x * deltaUV2.m_y - deltaUV1.m_y * deltaUV2.m_x);
	tangent = (deltaPos1 * deltaUV2.m_y - deltaPos2 * deltaUV1.m_y);
	tangent = tangent.normalized();
	binormal = (deltaPos2 * deltaUV1.m_x - deltaPos1 * deltaUV2.m_x);
	binormal = binormal.normalized();
}
