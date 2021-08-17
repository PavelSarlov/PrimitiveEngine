#pragma once
#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <Windows.h>
#include <algorithm>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>

#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Prerequisites.h"

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

public:
	SwapChainPtr createSwapChain(HWND hwnd, UINT width, UINT height, RenderSystem *system);
	DeviceContextPtr getImmediateDeviceContext();
	VertexBufferPtr createVertexBuffer(void *list_vertices, UINT size_vertex, UINT size_list, void *shader_byte_code, size_t size_byte_shader, RenderSystem *system);
	VertexShaderPtr createVertexShader(const void *shader_byte_code, size_t byte_code_size, RenderSystem *system);
	PixelShaderPtr createPixelShader(const void *shader_byte_code, size_t byte_code_size, RenderSystem *system);
	ConstantBufferPtr createConstantBuffer(void *buffer, UINT size_buffer, RenderSystem *system);
	IndexBufferPtr createIndexBuffer(void *list_indices, UINT size_list, RenderSystem *system);

public:
	bool compileVertexShader(const wchar_t *file_name, const char *entry_point_name, void **shader_byte_code, size_t *byte_code_size);
	bool compilePixelShader(const wchar_t *file_name, const char *entry_point_name, void **shader_byte_code, size_t *byte_code_size);
	void releaseCompiledShader();

private:
	DeviceContextPtr m_imm_device_context = nullptr;

private:
	ID3D11Device *m_d3d_device = nullptr;
	D3D_FEATURE_LEVEL m_feature_level = D3D_FEATURE_LEVEL_11_0;

private:
	IDXGIDevice *m_dxgi_device = nullptr;
	IDXGIAdapter *m_dxgi_adapter = nullptr;
	IDXGIFactory *m_dxgi_factory = nullptr;
	ID3D11DeviceContext *m_imm_context = nullptr;

private:
	ID3DBlob *m_blob = nullptr;

	ID3DBlob *m_vsblob = nullptr;
	ID3DBlob *m_psblob = nullptr;
	ID3D11VertexShader *m_vs = nullptr;
	ID3D11PixelShader *m_ps = nullptr;

private:
	friend class SwapChain;
	friend class VertexBuffer;
	friend class ConstantBuffer;
	friend class IndexBuffer;
	friend class VertexShader;
	friend class PixelShader;
};


#endif // !RENDERSYSTEM_H