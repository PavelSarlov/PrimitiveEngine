#pragma once

#ifndef DEVICECONTEXT_H
#define DEVICECONTEXT_H

#include <d3d11.h>

#include "SwapChain.h"
#include "VertexBuffer.h"

class SwapChain;
class VertexBuffer;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext *device_context);
	~DeviceContext();

public:
	bool Release();
	void ClearRenderTargetColor(SwapChain *swap_chain, float red, float green, float blue, float alpha);
	void SetVertexBuffer(VertexBuffer *vertex_buffer);
	void DrawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void SetViewPortSize(UINT width, UINT height);

private:
	ID3D11DeviceContext *m_device_context = nullptr;
};

#endif // !DEVICECONTEXT_H