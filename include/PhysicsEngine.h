#pragma once
#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <Prerequisites.h>
#include <Geometry.h>

class PhysicsEngine
{
private:
	PhysicsEngine();

public:
	~PhysicsEngine();

public:
	static PhysicsEngine *get();
	static void create();
	static void release();

public:
	ObjectPtr createObject(const ModelPtr &model, const std::vector<MaterialPtr> &materials, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale, bool collisive);
	ObjectPtr createObject(const ObjectPtr &object);

private:
	static PhysicsEngine *m_engine;
};

#endif // !PHYSICSENGINE