#pragma once
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Prerequisites.h"

#include <unordered_map>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

public:
	ResourcePtr createResourceFromFile(const wchar_t *file_path);

protected:
	virtual Resource *createResourceFromFileConcrete(const wchar_t *file_path) = 0;

private:
	std::unordered_map<std::wstring, ResourcePtr> m_map_resources;

};

#endif // !RESOURCEMANAGER_H