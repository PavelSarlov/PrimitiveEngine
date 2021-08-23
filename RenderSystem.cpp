#include "RenderSystem.h"

#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

RenderSystem::RenderSystem()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT hrs = 0;

	for(UINT driver_type_index = 0; driver_type_index < num_driver_types; ++driver_type_index)
	{
		hrs = D3D11CreateDevice(nullptr, driver_types[driver_type_index], nullptr, NULL, feature_levels, num_feature_levels, D3D11_SDK_VERSION,
			&this->m_d3d_device, &this->m_feature_level, &this->m_imm_context);

		if(SUCCEEDED(hrs))
		{
			break;
		}
	}

	if(FAILED(hrs))
	{
		throw std::exception("RenderSystem creation failed");
	}

	this->m_imm_device_context = std::make_shared<DeviceContext>(this->m_imm_context, this);

	this->m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void **)&m_dxgi_device);
	this->m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void **)&m_dxgi_adapter);
	this->m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_dxgi_factory);

	this->initRasterizerState();
}

RenderSystem::~RenderSystem()
{
	this->m_dxgi_device->Release();
	this->m_dxgi_adapter->Release();
	this->m_dxgi_factory->Release();

	this->m_d3d_device->Release();
}

SwapChainPtr RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	try
	{
		return std::make_shared<SwapChain>(hwnd, width, height, this);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
		return nullptr;
	}
}

DeviceContextPtr RenderSystem::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBufferPtr RenderSystem::createVertexBuffer(void *list_vertices, UINT size_vertex, UINT size_list, void *shader_byte_code, size_t size_byte_shader)
{
	try
	{
		return std::make_shared<VertexBuffer>(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader, this);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
		return nullptr;
	}
}

VertexShaderPtr RenderSystem::createVertexShader(const void *shader_byte_code, size_t byte_code_size)
{
	try
	{
		return std::make_shared<VertexShader>(shader_byte_code, byte_code_size, this);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
		return nullptr;
	}
}

PixelShaderPtr RenderSystem::createPixelShader(const void *shader_byte_code, size_t byte_code_size)
{
	try
	{
		return std::make_shared<PixelShader>(shader_byte_code, byte_code_size, this);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
		return nullptr;
	}
}

ConstantBufferPtr RenderSystem::createConstantBuffer(void *buffer, UINT size_buffer)
{
	try
	{
		return std::make_shared<ConstantBuffer>(buffer, size_buffer, this);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
		return nullptr;
	}
}

IndexBufferPtr RenderSystem::createIndexBuffer(void *list_indices, UINT size_list)
{
	try
	{
		return std::make_shared<IndexBuffer>(list_indices, size_list, this);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
		return nullptr;
	}
}

bool RenderSystem::compileVertexShader(const wchar_t *file_name, const char *entry_point_name, void **shader_byte_code, size_t *byte_code_size)
{
	ID3DBlob *error_blob = nullptr;
	if(FAILED(::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &this->m_blob, &error_blob)))
	{
		if(error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = this->m_blob->GetBufferPointer();
	*byte_code_size = this->m_blob->GetBufferSize();

	return true;
}

bool RenderSystem::compilePixelShader(const wchar_t *file_name, const char *entry_point_name, void **shader_byte_code, size_t *byte_code_size)
{
	ID3DBlob *error_blob = nullptr;
	if(FAILED(::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &this->m_blob, &error_blob)))
	{
		if(error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = this->m_blob->GetBufferPointer();
	*byte_code_size = this->m_blob->GetBufferSize();

	return true;
}

void RenderSystem::releaseCompiledShader()
{
	if(this->m_blob) this->m_blob->Release();
}

void RenderSystem::setRasterizerState(bool cull_front)
{
	this->m_imm_context->RSSetState(cull_front ? this->m_cull_front_state : this->m_cull_back_state);
}

void RenderSystem::initRasterizerState()
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_FRONT;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	this->m_d3d_device->CreateRasterizerState(&desc, &this->m_cull_front_state);

	desc.CullMode = D3D11_CULL_BACK;
	this->m_d3d_device->CreateRasterizerState(&desc, &this->m_cull_back_state);
}
