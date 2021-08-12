#include "VertexShader.h"

VertexShader::VertexShader()
{}

VertexShader::~VertexShader()
{}

void VertexShader::release()
{
	if(this->m_vs) this->m_vs->Release();
	delete this;
}

bool VertexShader::init(const void *shader_byte_code, size_t byte_code_size)
{
	if(FAILED(PrimitiveEngine::get()->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &this->m_vs)))
	{
		return false;
	}
	return true;
}
