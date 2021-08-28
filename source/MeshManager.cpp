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

#include <MeshManager.h>
#include <Mesh.h>

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
