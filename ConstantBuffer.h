#pragma once
#ifndef CONSTANTBUFFER_H
#define CONSTANTBUFFER_H

#include "PrimitiveEngine.h"
#include "DeviceContext.h"

#include <d3d11.h>

class DeviceContext;

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

public:
	bool load(void *buffer, UINT size_buffer);
	bool release();
	void update(DeviceContext *context, void *buffer);

private:
	ID3D11Buffer *m_buffer = nullptr;

private:
	friend class DeviceContext;
};

#endif // !CONSTANTBUFFER_H