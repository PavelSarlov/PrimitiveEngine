#include "MeshManager.h"

MeshManager::MeshManager()
{}

MeshManager::~MeshManager()
{}

MeshPtr MeshManager::createMeshFromFile(const wchar_t *file_path)
{
    return std::static_pointer_cast<Mesh>(createResourceFromFile(file_path));
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
