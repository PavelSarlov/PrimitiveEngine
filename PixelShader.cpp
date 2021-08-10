#include "PixelShader.h"

PixelShader::PixelShader()
{}

PixelShader::~PixelShader()
{}

void PixelShader::release()
{
	this->m_ps->Release();
}

bool PixelShader::init(const void *shader_byte_code, size_t byte_code_size)
{
	if(FAILED(PrimitiveEngine::get()->m_d3d_device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &this->m_ps)))
	{
		return false;
	}
	return true;
}
