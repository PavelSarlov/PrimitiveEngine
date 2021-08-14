#pragma once
#ifndef DEVICECONTEXT_H
#define DEVICECONTEXT_H

#include <d3d11.h>

#include "SwapChain.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

class SwapChain;
class VertexBuffer;
class ConstantBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext *device_context);
	~DeviceContext();

public:
	bool release();
	void setVertexBuffer(VertexBuffer *vertex_buffer);
	void setViewPortSize(UINT width, UINT height);
	void setVertexShader(VertexShader *vertex_shader);
	void setPixelShader(PixelShader *pixel_shader);
	void setConstantBuffer(VertexShader *vertex_shader, ConstantBuffer *buffer);
	void setConstantBuffer(PixelShader *pixel_shader, ConstantBuffer *buffer);
	void setIndexBuffer(IndexBuffer *index_buffer);

public:
	void clearRenderTargetColor(SwapChain *swap_chain, float red, float green, float blue, float alpha);
	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_index_location, UINT start_vertex_index);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

private:
	ID3D11DeviceContext *m_device_context = nullptr;

private:
	friend class ConstantBuffer;
};

#endif // !DEVICECONTEXT_H