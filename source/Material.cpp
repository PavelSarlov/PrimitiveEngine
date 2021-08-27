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
