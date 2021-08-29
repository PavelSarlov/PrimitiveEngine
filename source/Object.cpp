#include "Object.h"

Object::Object()
{}

Object::Object(const ModelPtr &model, const std::vector<MaterialPtr> &materials, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale, bool collisive) :
	m_model(model), m_materials(materials), m_position(position), m_rotation(rotation), m_scale(scale), m_collisive(collisive)
{}

Object::Object(const Object &other)
{
	*this = other;
}

Object &Object::operator=(const Object &other)
{
	if(this != &other)
	{
		this->m_model = other.m_model;
		this->m_materials = other.m_materials;
		this->m_position = other.m_position;
		this->m_rotation = other.m_rotation;
		this->m_scale = other.m_scale;
		this->m_collisive = other.m_collisive;
	}
	return *this;
}

Object::~Object()
{}

const ModelPtr &Object::getModel()
{
	return this->m_model;
}

const std::vector<MaterialPtr> &Object::getMaterials()
{
	return this->m_materials;
}

const Vector3 &Object::getPosition()
{
	return this->m_position;
}

const Vector3 &Object::getRotation()
{
	return this->m_rotation;
}

const Vector3 &Object::getScale()
{
	return this->m_scale;
}

bool Object::isCollisive()
{
	return this->m_collisive;
}

void Object::setModel(const ModelPtr &model)
{
	this->m_model = model;
}

void Object::setMaterials(const std::vector<MaterialPtr> &materials)
{
	this->m_materials = materials;
}

void Object::setPosition(const Vector3 &position)
{
	this->m_position = position;
}

void Object::setRotation(const Vector3 &rotation)
{
	this->m_rotation = rotation;
}

void Object::setScale(const Vector3 &scale)
{
	this->m_scale = scale;
}

void Object::setCollisive(bool collisive)
{
	this->m_collisive = collisive;
}
