#include "PixelShader.h"

PixelShader::PixelShader(const void *shader_byte_code, size_t byte_code_size, RenderSystem *system) : m_system(system)
{
	if(FAILED(this->m_system->m_d3d_device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &this->m_ps)))
	{
		throw std::exception("PixelShader creation failed");
	}
}

PixelShader::~PixelShader()
{
	if(this->m_ps) this->m_ps->Release();
}