#pragma once
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include "PrimitiveEngine.h"

#include <d3d11.h>

class DeviceContext;

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

public:
	bool load(void *list_indices, UINT size_list);
	bool release();
	UINT getSizeIndexList();

private:
	UINT m_size_list = 0;

private:
	ID3D11Buffer *m_buffer = nullptr;

private:
	friend class DeviceContext;
};

#endif // !INDEXBUFFER_H