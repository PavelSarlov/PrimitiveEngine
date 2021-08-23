#pragma once
#ifndef DEVICECONTEXT_H
#define DEVICECONTEXT_H

#include "Prerequisites.h"

#include <d3d11.h>

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext *device_context, RenderSystem *system);
	~DeviceContext();

public:
	void setViewPortSize(UINT width, UINT height);

	void setVertexShader(const VertexShaderPtr &vertex_shader);
	void setPixelShader(const PixelShaderPtr &pixel_shader);

	void setTexture(const VertexShaderPtr &vertex_shader, const TexturePtr &texture);
	void setTexture(const PixelShaderPtr &pixel_shader, const TexturePtr &texture);

	void setConstantBuffer(const VertexShaderPtr &vertex_shader, const ConstantBufferPtr &buffer);
	void setConstantBuffer(const PixelShaderPtr &pixel_shader, const ConstantBufferPtr &buffer);

	void setVertexBuffer(const VertexBufferPtr &vertex_buffer);
	void setIndexBuffer(const IndexBufferPtr &index_buffer);

public:
	void clearRenderTargetColor(const SwapChainPtr &swap_chain, float red, float green, float blue, float alpha);
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