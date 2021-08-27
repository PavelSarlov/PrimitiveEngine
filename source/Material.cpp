#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"

#include <exception>

Material::Material(const wchar_t *vertex_shader_path, const wchar_t *pixel_shader_path)
{
	// shaders data
	void *shader_byte_code = nullptr;
	size_t size_shader = 0;

	// create vertex shader
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(vertex_shader_path, "vsmain", &shader_byte_code, &size_shader);
	this->m_vertex_shader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	if(!this->m_vertex_shader)	throw std::runtime_error("Material vertex shader creation failed");

	// create pixel shader
	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(pixel_shader_path, "psmain", &shader_byte_code, &size_shader);
	this->m_pixel_shader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	if(!this->m_pixel_shader)	throw std::runtime_error("Material pixel shader creation failed");
}

Material::Material(const Material &other)
{
	*this = other;
}

Material &Material::operator=(const Material &other)
{
	if(this != &other)
	{
		this->m_constant_buffer = other.m_constant_buffer;
		this->m_cull_mode = other.m_cull_mode;
		this->m_pixel_shader = other.m_pixel_shader;
		this->m_vec_textures = other.m_vec_textures;
		this->m_vertex_shader = other.m_vertex_shader;
	}
	return *this;
}

Material::~Material()
{}

void Material::addTexture(const TexturePtr &texture)
{
	this->m_vec_textures.push_back(texture);
}

void Material::removeTexture(unsigned int index)
{
	if(index < this->m_vec_textures.size())
	{
		this->m_vec_textures.erase(this->m_vec_textures.begin() + index);
	}
}

void Material::setData(void *data, unsigned int size)
{
	if(!this->m_constant_buffer)
	{
		this->m_constant_buffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(data, size);
	}
	else
	{
		this->m_constant_buffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), data);
	}
}

void Material::setCullMode(CULL_MODE mode)
{
	this->m_cull_mode = mode;
}

CULL_MODE Material::getCullMode()
{
	return this->m_cull_mode;
}
