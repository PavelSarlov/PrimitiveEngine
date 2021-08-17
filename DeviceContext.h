#pragma once
#ifndef DEVICECONTEXT_H
#define DEVICECONTEXT_H

#include <d3d11.h>
#include <exception>

#include "SwapChain.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Prerequisites.h"

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext *device_context, RenderSystem *system);
	~DeviceContext();

public:
	void setVertexBuffer(VertexBufferPtr vertex_buffer);
	void setViewPortSize(UINT width, UINT height);
	void setVertexShader(VertexShaderPtr vertex_shader);
	void setPixelShader(PixelShaderPtr pixel_shader);
	void setConstantBuffer(VertexShaderPtr vertex_shader, ConstantBufferPtr buffer);
	void setConstantBuffer(PixelShaderPtr pixel_shader, ConstantBufferPtr buffer);
	void setIndexBuffer(IndexBufferPtr index_buffer);

public:
	void clearRenderTargetColor(SwapChainPtr swap_chain, float red, float green, float blue, float alpha);
	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_index_location, UINT start_vertex_index);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

private:
	ID3D11DeviceContext *m_device_context = nullptr;
	RenderSystem *m_system = nullptr;

private:
	friend class ConstantBuffer;
};

#endif // !DEVICECONTEXT_H