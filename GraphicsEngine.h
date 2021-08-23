#pragma once
#ifndef PRIMITIVEENGINE_H
#define PRIMITIVEENGINE_H

#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"

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
	MeshManager *getMeshManager();
	void getVertexLayoutShaderByteCodeAndSize(void **byte_code, size_t *size);

private:
	RenderSystem *m_render_system = nullptr;
	TextureManager *m_tex_manager = nullptr;
	MeshManager *m_mesh_manager = nullptr;
	static GraphicsEngine *m_engine;

private:
	unsigned char m_mesh_layout_byte_code[1024];
	size_t m_mesh_layout_size = 0;
};

#endif // !PRIMITIVEENGINE_H