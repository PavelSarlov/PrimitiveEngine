#include <GameObject.h>

GameObject::GameObject(const std::vector<MaterialPtr> &materials, const MeshPtr &mesh, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale, bool collisive) :
	m_materials(materials), m_mesh(mesh), m_position(position), m_rotation(rotation), m_scale(scale), m_collisive(collisive)
{}

GameObject::~GameObject()
{}

const std::vector<MaterialPtr> &GameObject::getMaterials()
{
	return this->m_materials;
}

const MeshPtr &GameObject::getMesh()
{
	return this->m_mesh;
}

const Vector3 &GameObject::getPosition()
{
	return this->m_position;
}

const Vector3 &GameObject::getRotation()
{
	return this->m_rotation;
}

const Vector3 &GameObject::getScale()
{
	return this->m_scale;
}

bool GameObject::isCollisive()
{
	return this->m_collisive;
}

void GameObject::setPosition(const Vector3 &position)
{
	this->m_position = position;
}

void GameObject::setRotation(const Vector3 &rotation)
{
	this->m_rotation = rotation;
}

void GameObject::setScale(const Vector3 &scale)
{
	this->m_scale = scale;
}

void GameObject::setCollision(bool collisive)
{
	this->m_collisive = collisive;
}
