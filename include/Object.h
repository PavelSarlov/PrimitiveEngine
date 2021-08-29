#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include <Geometry.h>
#include <Model.h>

class Object
{
public:
	Object();
	Object(const ModelPtr &model, const std::vector<MaterialPtr> &materials, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale, bool collisive);
	Object(const Object &other);
	Object &operator=(const Object &other);
	~Object();

public:
	const ModelPtr &getModel();
	const std::vector<MaterialPtr> &getMaterials();
	const Vector3 &getPosition();
	const Vector3 &getRotation();
	const Vector3 &getScale();
	bool isCollisive();

public:
	void setModel(const ModelPtr &model);
	void setMaterials(const std::vector<MaterialPtr> &materials);
	void setPosition(const Vector3 &position);
	void setRotation(const Vector3 &rotation);
	void setScale(const Vector3 &scale);
	void setCollisive(bool collisive);

private:
	ModelPtr m_model;
	std::vector<MaterialPtr> m_materials;
	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale = Vector3(1.0f);
	bool m_collisive = false;
};


#endif // !OBJECT_H