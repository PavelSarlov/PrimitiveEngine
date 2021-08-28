#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <Prerequisites.h>
#include <Geometry.h>
#include <vector>

class GameObject
{
public:
	GameObject(const std::vector<MaterialPtr> &materials, const MeshPtr &mesh, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale, bool collisive);
	~GameObject();

public:
	const std::vector<MaterialPtr> &getMaterials();
	const MeshPtr &getMesh();
	const Vector3 &getPosition();
	const Vector3 &getRotation();
	const Vector3 &getScale();
	bool isCollisive();

public:
	void setPosition(const Vector3 &pos);
	void setRotation(const Vector3 &rotation);
	void setScale(const Vector3 &scale);
	void setCollision(bool collisive);

private:
	std::vector<MaterialPtr> m_materials;
	MeshPtr m_mesh;
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;
	bool m_collisive = false;
};

#endif // !GAMEOBJECT_H
