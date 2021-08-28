#include <PhysicsEngine.h>
#include <GameObject.h>
#include <exception>

PhysicsEngine *PhysicsEngine::m_engine = nullptr;

PhysicsEngine::PhysicsEngine()
{}

PhysicsEngine::~PhysicsEngine()
{}

PhysicsEngine *PhysicsEngine::get()
{
	return PhysicsEngine::m_engine;
}

void PhysicsEngine::create()
{
	if(PhysicsEngine::m_engine) throw std::exception("PhysicsEngine already created");
	PhysicsEngine::m_engine = new PhysicsEngine();
}

void PhysicsEngine::release()
{
	if(!PhysicsEngine::m_engine) return;
	delete PhysicsEngine::m_engine;
}

GameObjectPtr PhysicsEngine::createGameObject(const std::vector<MaterialPtr> &materials, const MeshPtr &mesh, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale, bool collisive)
{
	return std::make_shared<GameObject>(materials, mesh, position, rotation, scale, collisive);
}