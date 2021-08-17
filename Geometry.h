#pragma once
#include <math.h>
#include <vector>
#include <iostream>

class Vector3;
class Vector4;
class Matrix3x3;
class Matrix4x4;
struct Vertex;
class Triangle;
class Mesh;

class Vector3
{
public:
	Vector3() : Vector3(0)
	{}

	Vector3(float a) : Vector3(a, a, a)
	{}

	Vector3(float x, float y, float z) : Vector3(std::vector<float>{x, y, z})
	{}

	Vector3(const std::vector<float> &p)
	{
		this->x = p[0];
		this->y = p[1];
		this->z = p[2];
	}

	Vector3(const Vector3 &other)
	{
		*this = other;
	}

	inline Vector3 &operator=(const Vector3 &other)
	{
		if(this != &other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
		}
		return *this;
	}

	inline float &operator[](int index)
	{
		switch(index)
		{
		case 0: return this->x;
		case 1: return this->y;
		case 2: return this->z;
		}
	}

	inline friend std::ostream &operator<<(std::ostream &out, const Vector3 &vec)
	{
		out << ' ' << vec.x << ' ' << vec.y << ' ' << vec.z;
		return out;
	}

	inline friend std::istream &operator>>(std::istream &in, Vector3 &vec)
	{
		in >> vec.x >> vec.y >> vec.z;
		return in;
	}

	inline Vector3 operator+(const Vector3 &other)
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	inline Vector3 operator-(const Vector3 &other)
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	inline float operator*(const Vector3 &other)
	{
		return x * other.x + y * other.y + z * other.z;
	}

	inline Vector3 operator*(float a)
	{
		return { x * a, y * a, z * a };
	}

	inline Vector3 operator/(float a)
	{
		if(a != 0.0f)
		{
			return { x / a, y / a, z / a };
		}
		return *this;
	}

	inline void operator+=(const Vector3 &other)
	{
		x += other.x; y += other.y; z += other.z;
	}

	inline void operator-=(const Vector3 &other)
	{
		x -= other.x; y -= other.y; z -= other.z;
	}

	inline void operator*=(float a)
	{
		x *= a; y *= a; z *= a;
	}

	inline void operator/=(float a)
	{
		if(a != 0.0f)
		{
			x /= a; y /= a; z /= a;
		}
	}

public:
	inline float length()
	{
		return sqrtf(x * x + y * y + z * z);
	}

	inline Vector3 normalized()
	{
		return Vector3(*this / this->length());
	}

	inline Vector3 crossProduct(const Vector3 &other)
	{
		return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
	}

	inline static Vector3 lerp(const Vector3 &start, const Vector3 &end, float delta)
	{
		return Vector3(start.x * (1.0f - delta) + end.x * (delta),
			start.y * (1.0f - delta) + end.y * (delta),
			start.z * (1.0f - delta) + end.z * (delta));
	}

public:
	float x, y, z;
};

class Vector4
{
public:
	Vector4() : Vector4(0)
	{}

	Vector4(float a) : Vector4(a, a, a)
	{}

	Vector4(float x, float y, float z) : Vector4(x, y, z, 1)
	{}

	Vector4(float x, float y, float z, float t) : Vector4(std::vector<float>{x, y, z, t})
	{}

	Vector4(const std::vector<float> &p)
	{
		this->x = p[0];
		this->y = p[1];
		this->z = p[2];
		this->t = p[3];
	}

	Vector4(const Vector4 &other)
	{
		*this = other;
	}

	inline Vector4 &operator=(const Vector4 &other)
	{
		if(this != &other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->t = other.t;
		}
		return *this;
	}

	inline float &operator[](int index)
	{
		switch(index)
		{
		case 0: return this->x;
		case 1: return this->y;
		case 2: return this->z;
		case 3: return this->t;
		default: return this->x;
		}
	}

	inline friend std::ostream &operator<<(std::ostream &out, const Vector4 &vec)
	{
		out << ' ' << vec.x << ' ' << vec.y << ' ' << vec.z << ' ' << vec.t;
		return out;
	}

	inline friend std::istream &operator>>(std::istream &in, Vector4 &vec)
	{
		in >> vec[0] >> vec[1] >> vec[2] >> vec[3];
		return in;
	}

	inline Vector4 operator+(const Vector4 &other)
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	inline Vector4 operator-(const Vector4 &other)
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	inline Vector4 operator*(float a)
	{
		return { x * a, y * a, z * a };
	}

	inline float operator*(const Vector4 &other)
	{
		return x * other.x + y * other.y + z * other.z;
	}

	inline Vector4 operator/(float a)
	{
		if(a != 0.0f)
			return { x / a, y / a, z / a };
		return *this;
	}

	inline void operator+=(const Vector4 &other)
	{
		x += other.x; y += other.y; z += other.z;
	}

	inline void operator-=(const Vector4 &other)
	{
		x -= other.x; y -= other.y; z -= other.z;
	}

	inline void operator*=(float a)
	{
		x *= a; y *= a; z *= a;
	}

	inline void operator/=(float a)
	{
		if(a != 0.0f)
		{
			x /= a; y /= a; z /= a;
		}
	}

public:
	inline float length()
	{
		this->dehomogenize();
		return sqrtf(x * x + y * y + z * z);
	}

	inline void dehomogenize()
	{
		if(t != 1.0f && t != 0.0f)
		{
			x /= t; y /= t; z /= t; t = 1;
		}
	}

	inline Vector4 normalized()
	{
		this->dehomogenize();
		float length = this->length();
		if(length != 0.0f)
			return *this / length;
		return *this;
	}

	inline Vector4 crossProduct(const Vector4 &other)
	{
		return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
	}

public:
	float x, y, z, t;
};

class Matrix3x3
{
public:
	Matrix3x3()
	{
		for(auto &r : this->m_mat)
		{
			for(auto &c : r)
			{
				c = 0;
			}
		}
	}

	Matrix3x3(float matrix[3][3])
	{
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				this->m_mat[i][j] = matrix[i][j];
			}
		}
	}

	Matrix3x3(const Matrix3x3 &other)
	{
		*this = other;
	}

	inline Matrix3x3 &operator=(const Matrix3x3 &other)
	{
		if(this != &other)
		{
			for(int i = 0; i < 3; i++)
			{
				for(int j = 0; j < 3; j++)
				{
					this->m_mat[i][j] = other.m_mat[i][j];
				}
			}
		}
		return *this;
	}

	inline friend std::ostream &operator<<(std::ostream &out, const Matrix3x3 &matrix)
	{
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				out << ' ' << matrix.m_mat[i][j];
			}
			out << std::endl;
		}
		return out;
	}

	inline friend std::istream &operator>>(std::istream &in, Matrix3x3 &matrix)
	{
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				in >> matrix.m_mat[i][j];
			}
		}
		return in;
	}

	inline float *operator[](int row)
	{
		if(row >= 0 && row < 3)
			return this->m_mat[row];
		return nullptr;
	}

	inline Matrix3x3 operator+(const Matrix3x3 &other)
	{
		Matrix3x3 result;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				result[i][j] = m_mat[i][j] + other.m_mat[i][j];
			}
		}
		return result;
	}

	inline Matrix3x3 operator-(const Matrix3x3 &other)
	{
		Matrix3x3 result;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				result[i][j] = m_mat[i][j] - other.m_mat[i][j];
			}
		}
		return result;
	}

	inline Matrix3x3 operator*(float a)
	{
		Matrix3x3 result;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				result[i][j] = m_mat[i][j] * a;
			}
		}
		return result;
	}

	inline Matrix3x3 operator*(const Matrix3x3 &other)
	{
		Matrix3x3 result;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				result[i][j] = m_mat[i][0] * other.m_mat[0][j] + m_mat[i][1] * other.m_mat[1][j] + m_mat[i][2] * other.m_mat[2][j];
			}
		}
		return result;
	}

	inline Matrix3x3 operator/(float a)
	{
		Matrix3x3 result;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				result[i][j] = m_mat[i][j] / a;
			}
		}
		return result;
	}

	inline void operator*=(float a)
	{
		*this = *this * a;
	}

	inline void operator*=(const Matrix3x3 &other)
	{
		*this = *this * other;
	}

	inline void operator/=(float a)
	{
		*this = *this / a;
	}

public:
	inline Matrix3x3 transposed()
	{
		Matrix3x3 result;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				result[i][j] = m_mat[j][i];
			}
		}
		return result;
	}

	inline static Matrix3x3 identityMatrix()
	{
		Matrix3x3 matrix;
		matrix[0][0] = 1.0f;
		matrix[1][1] = 1.0f;
		matrix[2][2] = 1.0f;
		return matrix;
	}

	inline float determinant()
	{
		return m_mat[0][0] * m_mat[1][1] * m_mat[2][2] +
			m_mat[0][1] * m_mat[1][2] * m_mat[2][0] +
			m_mat[0][2] * m_mat[2][1] * m_mat[1][0] -
			(m_mat[2][0] * m_mat[1][1] * m_mat[0][2] +
				m_mat[0][1] * m_mat[1][0] * m_mat[2][2] +
				m_mat[0][0] * m_mat[1][2] * m_mat[2][1]);
	}

	inline static Matrix3x3 rotationX(float angleRad)
	{
		Matrix3x3 matrix;
		matrix[0][0] = 1.0f;
		matrix[1][1] = cosf(angleRad);
		matrix[1][2] = -sinf(angleRad);
		matrix[2][1] = sinf(angleRad);
		matrix[2][2] = cosf(angleRad);
		return matrix;
	}

	inline static Matrix3x3 rotationY(float angleRad)
	{
		Matrix3x3 matrix;
		matrix[0][0] = cosf(angleRad);
		matrix[0][2] = sinf(angleRad);
		matrix[2][0] = -sinf(angleRad);
		matrix[2][2] = cosf(angleRad);
		matrix[1][1] = 1.0f;
		return matrix;
	}

	inline static Matrix3x3 rotationZ(float angleRad)
	{
		Matrix3x3 matrix;
		matrix[0][0] = cosf(angleRad);
		matrix[0][1] = -sinf(angleRad);
		matrix[1][0] = sinf(angleRad);
		matrix[1][1] = cosf(angleRad);
		matrix[2][2] = 1.0f;
		return matrix;
	}

	inline static Matrix3x3 transformationMatrix(const Vector4 &e1, const Vector4 &e2, const Vector4 &e3)
	{
		Matrix3x3 matrix;
		matrix[0][0] = e1.x; matrix[0][1] = e2.x; matrix[0][2] = e3.x;
		matrix[1][0] = e1.y; matrix[1][1] = e2.y; matrix[1][2] = e3.y;
		matrix[2][0] = e1.z; matrix[2][1] = e2.z; matrix[2][2] = e3.z;
		return matrix;
	}

public:
	float m_mat[3][3] = {};
};

class Matrix4x4
{
public:
	Matrix4x4()
	{
		for(auto &r : this->m_mat)
		{
			for(auto &c : r)
			{
				c = 0;
			}
		}
	}

	Matrix4x4(float matrix[4][4])
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				this->m_mat[i][j] = matrix[i][j];
			}
		}
	}

	Matrix4x4(const Matrix4x4 &other)
	{
		*this = other;
	}

	inline Matrix4x4 &operator=(const Matrix4x4 &other)
	{
		if(this != &other)
		{
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 4; j++)
				{
					this->m_mat[i][j] = other.m_mat[i][j];
				}
			}
		}
		return *this;
	}

	inline friend std::ostream &operator<<(std::ostream &out, const Matrix4x4 &matrix)
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				out << ' ' << matrix.m_mat[i][j];
			}
			out << std::endl;
		}
		return out;
	}

	inline friend std::istream &operator>>(std::istream &in, Matrix4x4 &matrix)
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				in >> matrix.m_mat[i][j];
			}
		}
		return in;
	}

	inline float *operator[](int row)
	{
		return this->m_mat[row];
	}

	inline Matrix4x4 operator+(const Matrix4x4 &other)
	{
		Matrix4x4 result;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				result[i][j] = m_mat[i][j] + other.m_mat[i][j];
			}
		}
		return result;
	}

	inline Matrix4x4 operator-(const Matrix4x4 &other)
	{
		Matrix4x4 result;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				result[i][j] = m_mat[i][j] - other.m_mat[i][j];
			}
		}
		return result;
	}

	inline Matrix4x4 operator*(float a)
	{
		Matrix4x4 result;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				result[i][j] = m_mat[i][j] * a;
			}
		}
		return result;
	}

	inline Matrix4x4 operator*(const Matrix4x4 &other)
	{
		Matrix4x4 result;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				result[i][j] = m_mat[i][0] * other.m_mat[0][j] + m_mat[i][1] * other.m_mat[1][j] + m_mat[i][2] * other.m_mat[2][j] + m_mat[i][3] * other.m_mat[3][j];
			}
		}
		return result;
	}

	inline Vector4 operator*(const Vector4 &v)
	{
		Vector4 result;
		for(int i = 0; i < 4; i++)
		{
			result[i] = m_mat[i][0] * v.x + m_mat[i][1] * v.y + m_mat[i][2] * v.z + m_mat[i][3] * v.t;
		}
		return result;
	}

	inline Matrix4x4 operator/(float a)
	{
		Matrix4x4 result;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				result[i][j] = m_mat[i][j] / a;
			}
		}
		return result;
	}

	inline void operator*=(float a)
	{
		*this = *this * a;
	}

	inline void operator*=(const Matrix4x4 &other)
	{
		*this = *this * other;
	}

	inline void operator/=(float a)
	{
		*this = *this / a;
	}

public:
	inline Matrix4x4 transposed()
	{
		Matrix4x4 result;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				result[i][j] = m_mat[j][i];
			}
		}
		return result;
	}

	inline static Matrix4x4 identityMatrix()
	{
		Matrix4x4 matrix;
		matrix[0][0] = 1.0f;
		matrix[1][1] = 1.0f;
		matrix[2][2] = 1.0f;
		matrix[3][3] = 1.0f;
		return matrix;
	}

	inline static Matrix4x4 rotationX(float angleRad)
	{
		Matrix4x4 matrix = Matrix4x4::identityMatrix();
		matrix[1][1] = cosf(angleRad);
		matrix[1][2] = sinf(angleRad);
		matrix[2][1] = -sinf(angleRad);
		matrix[2][2] = cosf(angleRad);
		return matrix;
	}

	inline static Matrix4x4 rotationY(float angleRad)
	{
		Matrix4x4 matrix = Matrix4x4::identityMatrix();
		matrix[0][0] = cosf(angleRad);
		matrix[0][2] = -sinf(angleRad);
		matrix[2][0] = sinf(angleRad);
		matrix[2][2] = cosf(angleRad);
		return matrix;
	}

	inline static Matrix4x4 rotationZ(float angleRad)
	{
		Matrix4x4 matrix = Matrix4x4::identityMatrix();
		matrix[0][0] = cosf(angleRad);
		matrix[0][1] = sinf(angleRad);
		matrix[1][0] = -sinf(angleRad);
		matrix[1][1] = cosf(angleRad);
		return matrix;
	}

	inline float determinant()
	{
		Matrix3x3 m1, m2, m3, m4;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				m1[i][j] = m_mat[i + 1][j + 1];
				m2[i][j] = m_mat[i + 1][j == 0 ? j : j + 1];
				m3[i][j] = m_mat[i + 1][j <= 1 ? j : j + 1];
				m4[i][j] = m_mat[i + 1][j];
			}
		}
		return m_mat[0][0] * m1.determinant() - m_mat[0][1] * m2.determinant() + m_mat[0][2] * m3.determinant() - m_mat[0][3] * m4.determinant();
	}

	inline Matrix4x4 inversedMatrix()
	{
		Matrix4x4 cij;

		for(int r = 0; r < 4; r++)
		{
			for(int c = 0; c < 4; c++)
			{
				Matrix3x3 adj;
				for(int i = 0, ii = 0; i < 4; i++)
				{
					if(i != r)
					{
						for(int j = 0, jj = 0; j < 4; j++)
						{
							if(j != c)
							{
								adj[ii][jj++] = m_mat[i][j];
							}
						}
						ii++;
					}
				}

				cij[r][c] = adj.determinant() * (float)(pow(-1, r + c));
			}
		}

		return (cij.transposed() * (1 / (*this).determinant()));
	}

	inline static Matrix4x4 projectionMatrix(float n, float f, float fov, float aspectRatio)
	{
		Matrix4x4 matrix;
		matrix[0][0] = aspectRatio / tanf(fov * 0.5f / 180.0f * 3.14159f);
		matrix[1][1] = 1.0f / tanf(fov / 2.0f * 3.1415f / 180.0f);
		matrix[2][2] = f / (f - n);
		matrix[2][3] = 1.0f;
		matrix[3][2] = -f * n / (f - n);
		return matrix;
	}

	inline static Matrix4x4 translationMatrix(float x, float y, float z)
	{
		return Matrix4x4::translationMatrix(Vector3(x, y, z));
	}

	inline static Matrix4x4 translationMatrix(const Vector3 &translation)
	{
		Matrix4x4 matrix = Matrix4x4::identityMatrix();
		matrix[3][0] = translation.x;
		matrix[3][1] = translation.y;
		matrix[3][2] = translation.z;
		return matrix;
	}

	inline void setTranslation(float x, float y, float z)
	{
		this->setTranslation(Vector3(x, y, z));
	}

	inline void setTranslation(const Vector3 &translation)
	{
		this->m_mat[3][0] = translation.x;
		this->m_mat[3][1] = translation.y;
		this->m_mat[3][2] = translation.z;
	}

	inline static Matrix4x4 orthogonalProjMatrix(float width, float height, float near_plane, float far_plane)
	{
		Matrix4x4 matrix = Matrix4x4::identityMatrix();
		matrix[0][0] = 2.0f / width;
		matrix[1][1] = 2.0f / height;
		matrix[2][2] = 1.0f / (far_plane - near_plane);
		matrix[3][2] = -(near_plane / (far_plane - near_plane));
		return matrix;
	}

	inline static Matrix4x4 perspectiveFovMatrix(float fov, float aspect, float zNear, float zFar)
	{
		float yScale = 1.0f / tanf(fov / 2.0f);
		float xScale = yScale / aspect;

		Matrix4x4 matrix = Matrix4x4::identityMatrix();
		matrix[0][0] = xScale;
		matrix[1][1] = yScale;
		matrix[2][2] = zFar / (zFar - zNear);
		matrix[2][3] = 1.0f;
		matrix[3][2] = (-zNear * zFar) / (zFar - zNear);
		return matrix;
	}

	inline static Matrix4x4 scaleMatrix(float x, float y, float z)
	{
		return Matrix4x4::scaleMatrix(Vector3(x, y, z));
	}

	inline static Matrix4x4 scaleMatrix(const Vector3 &scale)
	{
		Matrix4x4 matrix = Matrix4x4::identityMatrix();
		matrix[0][0] = scale.x;
		matrix[1][1] = scale.y;
		matrix[2][2] = scale.z;
		return matrix;
	}

	inline void setScale(float x, float y, float z)
	{
		return this->setScale(Vector3(x, y, z));
	}

	inline void setScale(const Vector3 &scale)
	{
		this->m_mat[0][0] = scale.x;
		this->m_mat[1][1] = scale.y;
		this->m_mat[2][2] = scale.z;
	}

	inline static Matrix4x4 pointAtMatrix(Vector4 camLocation, Vector4 camRight, Vector4 camUp, Vector4 camForward, Vector4 target)
	{
		/*Vector4 camForward = (target - camLocation).Normalize();
		Vector4	camUp = up - (camForward * (up * camForward)).Normalize();
		Vector4 camRight = camUp.crossProduct(camForward).Normalize();*/

		Matrix4x4 matrix;
		matrix[0][0] = camRight[0];		matrix[0][1] = camUp[0];	matrix[0][2] = camForward[0];	matrix[0][3] = target[0];
		matrix[1][0] = camRight[1];		matrix[1][1] = camUp[1];	matrix[1][2] = camForward[1];	matrix[1][3] = target[1];
		matrix[2][0] = camRight[2];		matrix[2][1] = camUp[2];	matrix[2][2] = camForward[2];	matrix[2][3] = target[2];
		matrix[3][0] = 0;				matrix[3][1] = 0;			matrix[3][2] = 0;				matrix[3][3] = 1;
		return matrix;
	}

	inline static Matrix4x4 lookAtMatrix(Vector4 camLocation, Vector4 camRight, Vector4 camUp, Vector4 camForward, Vector4 target)
	{
		Matrix4x4 pointAt = pointAtMatrix(camLocation, camRight, camUp, camForward, target);
		return pointAt.inversedMatrix();
	}

	inline static Matrix4x4 transformationMatrix(Vector4 e1, Vector4 e2, Vector4 e3)
	{
		Matrix4x4 matrix;
		matrix[0][0] = e1[0]; matrix[0][1] = e2[0]; matrix[0][2] = e3[0]; matrix[0][3] = 0;
		matrix[1][0] = e1[1]; matrix[1][1] = e2[1]; matrix[1][2] = e3[1]; matrix[1][3] = 0;
		matrix[2][0] = e1[2]; matrix[2][1] = e2[2]; matrix[2][2] = e3[2]; matrix[2][3] = 0;
		matrix[3][0] = 0;	  matrix[3][1] = 0;		matrix[3][2] = 0;	  matrix[3][3] = 1;
		return matrix;
	}

	inline static Vector4 rotateAroundX(Vector4 e1, Vector4 e2, Vector4 e3, Vector4 center, Vector4 point, float theta)
	{
		Matrix4x4 T = Matrix4x4::transformationMatrix(e1, e2, e3);
		Matrix4x4 A = Matrix4x4::rotationX(theta);

		Matrix4x4 matr = T * A * T.inversedMatrix();

		Vector4 result = matr * point + (center - matr * center);

		return result;
	}

	inline static Vector4 rotateAroundY(Vector4 e1, Vector4 e2, Vector4 e3, Vector4 center, Vector4 point, float theta)
	{
		Matrix4x4 T = Matrix4x4::transformationMatrix(e1, e2, e3);
		Matrix4x4 A = Matrix4x4::rotationY(theta);

		Matrix4x4 matr = T * A * T.inversedMatrix();

		Vector4 result = matr * point + (center - matr * center);

		return result;
	}

	inline static Vector4 rotateAroundZ(Vector4 e1, Vector4 e2, Vector4 e3, Vector4 center, Vector4 point, float theta)
	{
		Matrix4x4 T = Matrix4x4::transformationMatrix(e1, e2, e3);
		Matrix4x4 A = Matrix4x4::rotationZ(theta);

		Matrix4x4 matr = T * A * T.inversedMatrix();

		Vector4 result = matr * point + (center - matr * center);
		return result;
	}

	inline Vector3 getDirectionZ()
	{
		return { this->m_mat[2][0], this->m_mat[2][1], this->m_mat[2][2] };
	}

	inline Vector3 getDirectionX()
	{
		return { this->m_mat[0][0], this->m_mat[0][1], this->m_mat[0][2] };
	}

	inline Vector3 getTranslation()
	{
		return { this->m_mat[3][0], this->m_mat[3][1], this->m_mat[3][2] };
	}

private:
	inline void swapRows(float row1[], float row2[])
	{
		for(int i = 0; i < 4; i++)
		{
			float temp = row1[i];
			row1[i] = row2[i];
			row2[i] = temp;
		}
	}

public:
	float m_mat[4][4] = {};
};

struct Vertex
{
	/*Vertex() : Vertex(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f)
	{}

	Vertex(float x, float y, float z) : Vertex(x, y, z, 1.0f, 1.0f, 1.0f)
	{}

	Vertex(float x, float y, float z, float r, float g, float b)
	{
		this->pos = {x,y,z};
		this->color = {r,g,b};
	}

	Vertex(const Vertex &other)
	{
		*this = other;
	}

	Vertex &operator=(const Vertex &other)
	{
		if(this != &other)
		{
			this->pos = other.pos;
			this->color = other.color;
		}
		return *this;
	}*/

	Vector3 pos;
	Vector3 color;
	Vector3 color1;
};

__declspec(align(16))
struct Constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

//
//struct Triangle
//{
//	Triangle() : Triangle({Vertex(),Vertex(),Vertex()})
//	{}
//
//	Triangle(std::vector<Vertex> p)
//	{
//		for(int i = 0; i < 3; i++)
//		{
//			this->p[i] = p[i];
//		}
//	}
//
//	Triangle(const Triangle &other)
//	{
//		*this = other;
//	}
//
//	Triangle &operator=(const Triangle &other)
//	{
//		if(this != &other)
//		{
//			for(int i = 0; i < 3; i++)
//			{
//				this->p[i] = other.p[i];
//			}
//		}
//		return *this;
//	}
//
//	Vertex p[3];
//};
//
//struct Mesh
//{
//	Mesh() : Mesh(std::vector<Triangle>())
//	{}
//
//	Mesh(std::vector<Triangle> m)
//	{
//		this->m = m;
//	}
//
//	Mesh(const Mesh &other)
//	{
//		*this = other;
//	}
//
//	Mesh &operator=(const Mesh &other)
//	{
//		if(this != &other)
//		{
//			this->m = m;
//		}
//		return *this;
//	}
//
//	std::vector<Triangle> m;
//};
//
