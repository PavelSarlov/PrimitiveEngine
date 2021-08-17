#pragma once
#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H

#include "RenderSystem.h"
#include "DeviceContext.h"
#include "Prerequisites.h"

#include <exception>
#include <d3d11.h>

class DeviceContext;

class ConstantBuffer
{
public:
	ConstantBuffer(void *buffer, UINT size_buffer, RenderSystem *system);
	~ConstantBuffer();

public:
	void update(DeviceContext *context, void *buffer);

private:
	ID3D11Buffer *m_buffer = nullptr;
	RenderSystem *m_system = nullptr;

private:
	friend class DeviceContext;
};

#endif // !CONSTANTBUFFER_H