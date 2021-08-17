#include "RenderSystem.h"

RenderSystem::RenderSystem()
{}

RenderSystem::~RenderSystem()
{}

bool RenderSystem::init()
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
		return false;
	}

	this->m_imm_device_context = new DeviceContext(this->m_imm_context, this);

	this->m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void **)&m_dxgi_device);
	this->m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void **)&m_dxgi_adapter);
	this->m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_dxgi_factory);

	return true;
}

bool RenderSystem::release()
{
	if(this->m_vs) this->m_vs->Release();
	if(this->m_ps) this->m_ps->Release();

	if(this->m_vsblob) this->m_vsblob->Release();
	if(this->m_psblob) this->m_psblob->Release();

	if(this->m_dxgi_device) this->m_dxgi_device->Release();
	if(this->m_dxgi_adapter) this->m_dxgi_adapter->Release();
	if(this->m_dxgi_factory) this->m_dxgi_factory->Release();

	delete this->m_imm_device_context;
	if(this->m_d3d_device) this->m_d3d_device->Release();

	return true;
}

SwapChain *RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height, RenderSystem *system)
{
	try
	{
		return new SwapChain(hwnd, width, height, this);
	}
	catch(std::exception e)
	{
		std::cout << e.what();
		return nullptr;
	}
}

DeviceContext *RenderSystem::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBuffer *RenderSystem::createVertexBuffer(void *list_vertices, UINT size_vertex, UINT size_list, void *shader_byte_code, size_t size_byte_shader, RenderSystem *system)
{
	try
	{
		return new VertexBuffer(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader, this);
	}
	catch(std::exception e)
	{
		std::cout << e.what();
		return nullptr;
	}
}

VertexShader *RenderSystem::createVertexShader(const void *shader_byte_code, size_t byte_code_size, RenderSystem *system)
{
	try
	{
		return new VertexShader(shader_byte_code, byte_code_size, this);
	}
	catch(std::exception e)
	{
		std::cout << e.what();
		return nullptr;
	}
}

PixelShader *RenderSystem::createPixelShader(const void *shader_byte_code, size_t byte_code_size, RenderSystem *system)
{
	try
	{
		return new PixelShader(shader_byte_code, byte_code_size, this);
	}
	catch(std::exception e)
	{
		std::cout << e.what();
		return nullptr;
	}
}

ConstantBuffer *RenderSystem::createConstantBuffer(void *buffer, UINT size_buffer, RenderSystem *system)
{
	try
	{
		return new ConstantBuffer(buffer, size_buffer, this);
	}
	catch(std::exception e)
	{
		std::cout << e.what();
		return nullptr;
	}
}

IndexBuffer *RenderSystem::createIndexBuffer(void *list_indices, UINT size_list, RenderSystem *system)
{
	try
	{
		return new IndexBuffer(list_indices, size_list, this);
	}
	catch(std::exception e)
	{
		std::cout << e.what();
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