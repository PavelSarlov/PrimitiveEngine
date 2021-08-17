#include "GraphicsEngine.h"

GraphicsEngine *GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try
	{
		this->m_render_system = new RenderSystem();
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
		throw std::exception("GraphicsEngine initialization failed");
	}
}

GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::m_engine = nullptr;
	delete this->m_render_system;
}

GraphicsEngine *GraphicsEngine::get()
{
	return GraphicsEngine::m_engine;
}

void GraphicsEngine::create()
{
	if(GraphicsEngine::m_engine) throw std::exception("GraphicsEngine already created");
	GraphicsEngine::m_engine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if(!GraphicsEngine::m_engine) return;
	delete GraphicsEngine::m_engine;
}

RenderSystem *GraphicsEngine::getRenderSystem()
{
	return this->m_render_system;
}
