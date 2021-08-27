#pragma once
#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class Resource
{
public:
	Resource(const wchar_t *full_path);
	virtual ~Resource();

protected:
	std::wstring m_full_path;
};

#endif // !RESOURCE_H