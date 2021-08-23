#pragma once
#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "ResourceManager.h"

class MeshManager : public ResourceManager
{
public:
	MeshManager();
	~MeshManager();

	MeshPtr createMeshFromFile(const wchar_t *file_path);

protected:
	virtual Resource *createResourceFromFileConcrete(const wchar_t *file_path);
};

#endif