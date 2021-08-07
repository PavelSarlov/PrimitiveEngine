#include "VertexShader.h"

VertexShader::VertexShader()
{}

VertexShader::~VertexShader()
{}

void VertexShader::Release()
{
	this->m_vs->Release();
	delete this;
}

bool VertexShader::Init(const void *shader_byte_code, size_t byte_code_size)
{
	if(FAILED(PrimitiveEngine::Get()->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &this->m_vs)))
	{
		return false;
	}
	return true;
}
