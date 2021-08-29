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

#pragma once
#ifndef PRIMITIVEENGINE_H
#define PRIMITIVEENGINE_H

#include <Prerequisites.h>
#include <RenderSystem.h>
#include <TextureManager.h>
#include <ModelManager.h>

class GraphicsEngine
{
private:
	GraphicsEngine();

public:
	~GraphicsEngine();

public:
	static GraphicsEngine *get();
	static void create();
	static void release();

public:
	RenderSystem *getRenderSystem();
	TextureManager *getTextureManager();
	ModelManager *getModelManager();

public:
	void getVertexLayoutShaderByteCodeAndSize(void **byte_code, size_t *size);
	void setMaterial(const MaterialPtr &material);
	MaterialPtr createMaterial(const wchar_t *vertex_shader_path, const wchar_t *pixel_shader_path);
	MaterialPtr createMaterial(const MaterialPtr &material);

private:
	RenderSystem *m_render_system = nullptr;
	TextureManager *m_tex_manager = nullptr;
	ModelManager *m_model_manager = nullptr;
	static GraphicsEngine *m_engine;

private:
	unsigned char m_mesh_layout_byte_code[1024];
	size_t m_mesh_layout_size = 0;
};

#endif // !PRIMITIVEENGINE_H