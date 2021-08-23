#pragma once
#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H

#include "Prerequisites.h"

#include <d3d11.h>

class DeviceContext;

class ConstantBuffer
{
public:
	ConstantBuffer(void *buffer, UINT size_buffer, RenderSystem *system);
	~ConstantBuffer();

public:
	void update(DeviceContextPtr context, void *buffer);

private:
	ID3D11Buffer *m_buffer = nullptr;
	RenderSystem *m_system = nullptr;

private:
	friend class DeviceContext;
};

#endif // !CONSTANTBUFFER_H