#include "GraphicsEngine.h"

GraphicsEngine *GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try
	{
		this->m_render_system = new RenderSystem();
		this->m_tex_manager = new TextureManager();
		this->m_mesh_manager = new MeshManager();
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
		throw std::exception("GraphicsEngine initialization failed");
	}

	void *shader_byte_code = nullptr;
	size_t size_shader = 0;

	this->m_render_system->compileVertexShader(L"VertexMeshLayoutShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	::memcpy(this->m_mesh_layout_byte_code, shader_byte_code, size_shader);
	this->m_mesh_layout_size = size_shader;
	this->m_render_system->releaseCompiledShader();
}

GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::m_engine = nullptr;
	delete this->m_mesh_manager;
	delete this->m_tex_manager;
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

TextureManager *GraphicsEngine::getTextureManager()
{
	return this->m_tex_manager;
}

MeshManager *GraphicsEngine::getMeshManager()
{
	return this->m_mesh_manager;
}

void GraphicsEngine::getVertexLayoutShaderByteCodeAndSize(void **byte_code, size_t *size)
{
	*byte_code = this->m_mesh_layout_byte_code;
	*size = this->m_mesh_layout_size;
}
