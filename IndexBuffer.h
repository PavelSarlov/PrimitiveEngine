#pragma once
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "RenderSystem.h"
#include "Prerequisites.h"

#include <d3d11.h>
#include <exception>

class DeviceContext;

class IndexBuffer
{
public:
	IndexBuffer(void *list_indices, UINT size_list, RenderSystem *system);
	~IndexBuffer();

public:
	UINT getSizeIndexList();

private:
	UINT m_size_list = 0;

private:
	ID3D11Buffer *m_buffer = nullptr;
	RenderSystem *m_system = nullptr;

private:
	friend class DeviceContext;
};

#endif // !INDEXBUFFER_H