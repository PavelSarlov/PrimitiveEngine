#include "MeshManager.h"
#include "Mesh.h"

#include <iostream>

MeshManager::MeshManager()
{}

MeshManager::~MeshManager()
{}

MeshPtr MeshManager::createMeshFromFile(const wchar_t *file_path)
{
	return std::static_pointer_cast<Mesh>(createResourceFromFile(file_path));
}

MeshPtr MeshManager::createMesh(
	VertexMesh *vertex_list_data, unsigned int vertex_list_size,
	unsigned int *index_list_data, unsigned int index_list_size,
	MaterialSlot *material_slot_list, unsigned int material_slot_list_size)
{
	try
	{
		return MeshPtr(new Mesh(vertex_list_data, vertex_list_size, index_list_data, index_list_size, material_slot_list, material_slot_list_size));
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	return MeshPtr(nullptr);
}

Resource *MeshManager::createResourceFromFileConcrete(const wchar_t *file_path)
{
	try
	{
		return new Mesh(file_path);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	return nullptr;
}
