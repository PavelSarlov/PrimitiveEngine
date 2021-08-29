#pragma once
#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <ResourceManager.h>

class ModelManager : public ResourceManager
{
public:
	ModelManager();
	~ModelManager();

public:
	ModelPtr createModelFromFile(const wchar_t *file_path);

protected:
	virtual Resource *createResourceFromFileConcrete(const wchar_t *file_path);
};


#endif // !MODELMANAGER_H