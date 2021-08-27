#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Prerequisites.h"
#include "GraphicsEngine.h"

#include <vector>

enum CULL_MODE
{
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK
};

class Material
{
public:
	Material(const wchar_t *vertex_shader_path, const wchar_t *pixel_shader_path);
	Material(const Material &other);
	Material &operator=(const Material &other);
	~Material();

public:
	void addTexture(const TexturePtr &texture);
	void removeTexture(unsigned int index);
	
	void setData(void *data, unsigned int size);
	
	void setCullMode(CULL_MODE mode);
	CULL_MODE getCullMode();

private:
	VertexShaderPtr m_vertex_shader;
	PixelShaderPtr m_pixel_shader;
	ConstantBufferPtr m_constant_buffer;
	std::vector<TexturePtr> m_vec_textures;
	CULL_MODE m_cull_mode = CULL_MODE_BACK;

private:
	friend class GraphicsEngine;
};

#endif // !MATERIAL_H