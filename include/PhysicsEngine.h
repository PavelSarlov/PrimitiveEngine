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
	GameObjectPtr createGameObject(const std::vector<MaterialPtr> &materials, const MeshPtr &mesh, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale, bool collisive);

private:
	static PhysicsEngine *m_engine;
};

#endif // !PHYSICSENGINE