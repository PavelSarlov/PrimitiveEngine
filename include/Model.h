#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <Prerequisites.h>
#include <Geometry.h>
#include <Resource.h>
#include <vector>

class Model : public Resource
{
public:
	Model(const wchar_t *full_path);
	~Model();

public:
	const std::vector<MeshPtr> &getMeshes();

private:
	void computeTangents(
		const Vector3 &v0, const Vector3 &v1, const Vector3 &v2,
		const Vector2 &t0, const Vector2 &t1, const Vector2 &t2,
		Vector3 &tangent, Vector3 &binormal
	);

private:
	std::vector<MeshPtr> m_meshes;
};

#endif // !Object_H
