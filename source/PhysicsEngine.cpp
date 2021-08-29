#include <PhysicsEngine.h>
#include <Object.h>
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

ObjectPtr PhysicsEngine::createObject(const ModelPtr &model, const std::vector<MaterialPtr> &materials, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale, bool collisive)
{
	return std::make_shared<Object>(model, materials, position, rotation, scale, collisive);
}

ObjectPtr PhysicsEngine::createObject(const ObjectPtr &object)
{
	return std::make_shared<Object>(*(object.get()));
}
