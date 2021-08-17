#pragma once
#ifndef PRIMITIVEENGINE_H
#define PRIMITIVEENGINE_H

#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

public:
	bool init();
	bool release();

public:
	static GraphicsEngine *get();

public:
	RenderSystem *getRenderSystem();

private:
	RenderSystem *m_render_system = nullptr;
};

#endif // !PRIMITIVEENGINE_H