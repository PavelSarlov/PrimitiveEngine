#pragma once
#ifndef PRIMITIVEENGINE_H
#define PRIMITIVEENGINE_H

#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"

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

private:
	RenderSystem *m_render_system = nullptr;
	TextureManager *m_tex_manager = nullptr;
	static GraphicsEngine *m_engine;
};

#endif // !PRIMITIVEENGINE_H