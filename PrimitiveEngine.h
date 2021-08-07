#pragma once
#ifndef PRIMITIVEENGINE_H
#define PRIMITIVEENGINE_H

#include <stdint.h>
#include <Windows.h>
#include <stdio.h>
#include <algorithm>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;

class PrimitiveEngine
{
public:
	PrimitiveEngine();
	~PrimitiveEngine();

public:
	bool Init();
	bool Release();

public:
	static PrimitiveEngine *Get();

public:
	SwapChain *CreateSwapChain();
	DeviceContext *GetImmediateDeviceContext();
	VertexBuffer *CreateVertexBuffer();
	VertexShader *CreateVertexShader(const void *shader_byte_code, size_t byte_code_size);

public:
	bool CompileVertexShader(const wchar_t *file_name, const char *entry_point_name, void **shader_byte_code, size_t *byte_code_size);
	void ReleaseCompiledShader();
	bool CreateShaders();
	bool SetShaders();


	//public:
	//	ECODE DrawPixel(HDC hdc, COORD x, COORD y, COLORREF c);
	//	ECODE DrawLine(HDC hdc, COORD x1, COORD y1, COORD x2, COORD y2, COLORREF c);
	//	ECODE DrawTriangle(HDC hdc, COORD x1, COORD y1, COORD x2, COORD y2, COORD x3, COORD y3, COLORREF c);
	//	ECODE FillTriangle(HDC hdc, COORD x1, COORD y1, COORD x2, COORD y2, COORD x3, COORD y3, COLORREF c);
	//	ECODE DrawCircle(HDC hdc, COORD x1, COORD y1, COORD r, COLORREF c);
	//	ECODE FillCircle(HDC hdc, COORD x1, COORD y1, COORD r, COLORREF c);
	//	ECODE FillRectangle(HDC hdc, COORD x1, COORD y1, COORD x2, COORD y2, COLORREF c);

private:
	void SortLeftRightTriangle(COORD &x1, COORD &y1, COORD &x2, COORD &y2, COORD &x3, COORD &y3);

private:
	DeviceContext *m_imm_device_context = nullptr;

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
	friend class VertexShader;
};

#endif // !PRIMITIVEENGINE_H