#include "VertexShader.h"
#include "RenderSystem.h"

#include <exception>

VertexShader::VertexShader(const void *shader_byte_code, size_t byte_code_size, RenderSystem *system) : m_system(system)
{
	if(FAILED(this->m_system->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &this->m_vs)))
	{
		throw std::exception("VertexShader creation failed");
	}
}

VertexShader::~VertexShader()
{
	if(this->m_vs) this->m_vs->Release();
}
