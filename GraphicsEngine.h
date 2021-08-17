#pragma once
#ifndef PRIMITIVEENGINE_H
#define PRIMITIVEENGINE_H

#include "Prerequisites.h"
#include "RenderSystem.h"

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

private:
	RenderSystem *m_render_system = nullptr;
	static GraphicsEngine *m_engine;
};

#endif // !PRIMITIVEENGINE_H