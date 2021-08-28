// MIT License
// 
// C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)
// 
// Copyright (c) 2019-2021, PardCode
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <Prerequisites.h>

#include <Windows.h>
#include <algorithm>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

public:
	SwapChainPtr createSwapChain(HWND hwnd, UINT width, UINT height);
	DeviceContextPtr getImmediateDeviceContext();
	VertexBufferPtr createVertexBuffer(void *list_vertices, UINT size_vertex, UINT size_list, void *shader_byte_code, size_t size_byte_shader);
	VertexShaderPtr createVertexShader(const void *shader_byte_code, size_t byte_code_size);
	PixelShaderPtr createPixelShader(const void *shader_byte_code, size_t byte_code_size);
	ConstantBufferPtr createConstantBuffer(void *buffer, UINT size_buffer);
	IndexBufferPtr createIndexBuffer(void *list_indices, UINT size_list);

public:
	bool compileVertexShader(const wchar_t *file_name, const char *entry_point_name, void **shader_byte_code, size_t *byte_code_size);
	bool compilePixelShader(const wchar_t *file_name, const char *entry_point_name, void **shader_byte_code, size_t *byte_code_size);
	void releaseCompiledShader();
	void setRasterizerState(bool cull_front);

private:
	void initRasterizerState();

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
	ID3D11RasterizerState *m_cull_front_state = nullptr;
	ID3D11RasterizerState *m_cull_back_state = nullptr;

private:
	friend class SwapChain;
	friend class VertexBuffer;
	friend class ConstantBuffer;
	friend class IndexBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class Texture;
};


#endif // !RENDERSYSTEM_H