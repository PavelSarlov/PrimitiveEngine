#pragma once
#include <math.h>
#include <vector>
#include <iostream>

struct Vector3;
struct Vector4;
struct Matrix3;
struct Matrix4;
struct Vertex;
struct Triangle;
struct Mesh;

struct Vector3
{
	Vector3() : Vector3(0)
	{}

	Vector3(float a) : Vector3(a, a, a)
	{}

	Vector3(float x, float y, float z) : Vector3(std::vector<float>{ x, y, z })
	{}

	Vector3(std::vector<float> p)
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
	inline friend std::ostream &operator<<(std::ostream &out, Vector3 &vec)
	{
		out << ' ' << vec.x << ' ' << vec.y << ' ' << vec.z;
		return out;
	}
	inline friend std::istream &operator>>(std::istream &in, Vector3 &vec)
	{
		in >> vec.x >> vec.y >> vec.z;
		return in;
	}
	inline Vector3 operator+(Vector3 other)
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	inline Vector3 operator-(Vector3 other)
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	inline float operator*(Vector3 other)
	{
		return x * other.x + y * other.y + z * other.z;
	}
	inline Vector3 operator*(const float a)
	{
		return Vector3(x * a, y * a, z * a);
	}
	inline Vector3 operator/(const float a)
	{
		if(a != 0.0f)
		{
			return Vector3(x / a, y / a, z / a);
		}
		return *this;
	}

	inline void operator+=(Vector3 other)
	{
		x += other.x; y += other.y; z += other.z;
	}
	inline void operator-=(Vector3 other)
	{
		x -= other.x; y -= other.y; z -= other.z;
	}
	inline void operator*=(const float a)
	{
		x *= a; y *= a; z *= a;
	}
	inline void operator/=(const float a)
	{
		if(a != 0.0f)
		{
			x /= a; y /= a; z /= a;
		}
	}
	inline float Length()
	{
		return sqrtf(x * x + y * y + z * z);
	}
	inline Vector3 Normalized()
	{
		return Vector3(*this / this->Length());
	}
	inline Vector3 CrossProduct(Vector3 other)
	{
		return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}

	float x, y, z;
};

struct Vector4
{
	Vector4() : Vector4(0)
	{}

	Vector4(float a) : Vector4(a, a, a)
	{}

	Vector4(float x, float y, float z) : Vector4(x, y, z, 1)
	{}

	Vector4(float x, float y, float z, float t) : Vector4(std::vector<float>{ x, y, z, t })
	{}

	Vector4(std::vector<float> p)
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
	inline friend std::ostream &operator<<(std::ostream &out, Vector4 &vec)
	{
		out << ' ' << vec.x << ' ' << vec.y << ' ' << vec.z << ' ' << vec.t;
		return out;
	}
	inline friend std::istream &operator>>(std::istream &in, Vector4 &vec)
	{
		in >> vec[0] >> vec[1] >> vec[2] >> vec[3];
		return in;
	}
	inline Vector4 operator+(Vector4 other)
	{
		this->Dehomogenize();
		return Vector4(x + other.x, y + other.y, z + other.z);
	}
	inline Vector4 operator-(Vector4 other)
	{
		this->Dehomogenize();
		return Vector4(x - other.x, y - other.y, z - other.z);
	}
	inline Vector4 operator*(const float a)
	{
		this->Dehomogenize();
		return Vector4(x * a, y * a, z * a);
	}
	inline float operator*(Vector4 other)
	{
		this->Dehomogenize();
		other.Dehomogenize();
		return x * other.x + y * other.y + z * other.z;
	}
	inline Vector4 operator/(const float a)
	{
		this->Dehomogenize();
		if(a != 0.0f)
			return Vector4(x / a, y / a, z / a);
		return *this;
	}
	inline void operator+=(Vector4 other)
	{
		this->Dehomogenize();
		other.Dehomogenize();
		x += other.x; y += other.y; z += other.z;
	}
	inline void operator-=(Vector4 other)
	{
		this->Dehomogenize();
		other.Dehomogenize();
		x -= other.x; y -= other.y; z -= other.z;
	}
	inline void operator*=(const float a)
	{
		this->Dehomogenize();
		x *= a; y *= a; z *= a;
	}
	inline void operator/=(const float a)
	{
		this->Dehomogenize();
		if(a != 0.0f)
		{
			x /= a; y /= a; z /= a;
		}
	}
	inline float Length()
	{
		this->Dehomogenize();
		return sqrtf(x * x + y * y + z * z);
	}
	inline void Dehomogenize()
	{
		if(t != 1.0f && t != 0.0f)
		{
			x /= t; y /= t; z /= t; t = 1;
		}
	}
	inline Vector4 Normalized()
	{
		this->Dehomogenize();
		float length = this->Length();
		if(length != 0.0f)
			return *this / length;
		return *this;
	}
	inline Vector4 CrossProduct(Vector4 other)
	{
		other.Dehomogenize();
		this->Dehomogenize();
		return Vector4(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}

	float x, y, z, t;
};

struct Matrix3
{
	Matrix3()
	{
		for(auto &r : this->m)
		{
			for(auto &c : r)
			{
				c = 0;
			}
		}
	}
	Matrix3(float matrix[3][3])
	{
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				this->m[i][j] = matrix[i][j];
			}
		}
	}
	Matrix3(const Matrix3 &other)
	{
		*this = other;
	}

	inline Matrix3 &operator=(const Matrix3 &other)
	{
		if(this != &other)
		{
			for(int i = 0; i < 3; i++)
			{
				for(int j = 0; j < 3; j++)
				{
					this->m[i][j] = other.m[i][j];
				}
			}
		}
		return *this;
	}

	inline friend std::ostream &operator<<(std::ostream &out, Matrix3 &matrix)
	{
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				out << ' ' << matrix[i][j];
			}
			out << std::endl;
		}
		return out;
	}

	inline friend std::istream &operator>>(std::istream &in, Matrix3 &matrix)
	{
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				in >> matrix[i][j];
			}
		}
		return in;
	}

	inline float *operator[](int row)
	{
		if(row >= 0 && row < 3)
			return this->m[row];
	}

	inline Matrix3 operator+(Matrix3 other)
	{
		Matrix3 result;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				result[i][j] = m[i][j] + other.m[i][j];
			}
		}
		return result;
	}

	inline Matrix3 operator-(Matrix3 other)
	{
		Matrix3 result;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				result[i][j] = m[i][j] - other.m[i][j];
			}
		}
		return result;
	}

	inline Matrix3 operator*(float a)
	{
		Matrix3 result;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				result[i][j] = m[i][j] * a;
			}
		}
		return result;
	}

	inline Matrix3 operator*(Matrix3 other)
	{
		Matrix3 result;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				result[i][j] = m[i][0] * other[0][j] + m[i][1] * other[1][j] + m[i][2] * other[2][j];
			}
		}
		return result;
	}

	inline Matrix3 operator/(float a)
	{
		Matrix3 result;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				result[i][j] = m[i][j] / a;
			}
		}
		return result;
	}

	inline void operator*=(float a)
	{
		*this = *this * a;
	}

	inline void operator*=(Matrix3 other)
	{
		*this = *this * other;
	}

	inline void operator/=(float a)
	{
		*this = *this / a;
	}

	inline Matrix3 Transposed()
	{
		Matrix3 result;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				result[i][j] = m[j][i];
			}
		}
		return result;
	}

	inline static Matrix3 IdentityMatrix()
	{
		Matrix3 matrix;
		matrix[0][0] = 1.0f;
		matrix[1][1] = 1.0f;
		matrix[2][2] = 1.0f;
		return matrix;
	}

	inline float Determinant()
	{
		return m[0][0] * m[1][1] * m[2][2] +
			m[0][1] * m[1][2] * m[2][0] +
			m[0][2] * m[2][1] * m[1][0] -
			(m[2][0] * m[1][1] * m[0][2] +
				m[0][1] * m[1][0] * m[2][2] +
				m[0][0] * m[1][2] * m[2][1]);
	}

	inline static Matrix3 RotationX(float angleRad)
	{
		Matrix3 matrix;
		matrix[0][0] = 1.0f;
		matrix[1][1] = cosf(angleRad);
		matrix[1][2] = -sinf(angleRad);
		matrix[2][1] = sinf(angleRad);
		matrix[2][2] = cosf(angleRad);
		return matrix;
	}

	inline static Matrix3 RotationY(float angleRad)
	{
		Matrix3 matrix;
		matrix[0][0] = cosf(angleRad);
		matrix[0][2] = sinf(angleRad);
		matrix[2][0] = -sinf(angleRad);
		matrix[2][2] = cosf(angleRad);
		matrix[1][1] = 1.0f;
		return matrix;
	}

	inline static Matrix3 RotationZ(float angleRad)
	{
		Matrix3 matrix;
		matrix[0][0] = cosf(angleRad);
		matrix[0][1] = -sinf(angleRad);
		matrix[1][0] = sinf(angleRad);
		matrix[1][1] = cosf(angleRad);
		matrix[2][2] = 1.0f;
		return matrix;
	}

	inline static Matrix3 TransformationMatrix(Vector4 e1, Vector4 e2, Vector4 e3)
	{
		Matrix3 matrix;
		matrix[0][0] = e1[0]; matrix[0][1] = e2[0]; matrix[0][2] = e3[0];
		matrix[1][0] = e1[1]; matrix[1][1] = e2[1]; matrix[1][2] = e3[1];
		matrix[2][0] = e1[2]; matrix[2][1] = e2[2]; matrix[2][2] = e3[2];
		return matrix;
	}

	float m[3][3];
};

struct Matrix4
{
	Matrix4()
	{
		for(auto &r : this->m)
		{
			for(auto &c : r)
			{
				c = 0;
			}
		}
	}
	Matrix4(float matrix[4][4])
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				this->m[i][j] = matrix[i][j];
			}
		}
	}
	Matrix4(const Matrix4 &other)
	{
		*this = other;
	}

	inline Matrix4 &operator=(const Matrix4 &other)
	{
		if(this != &other)
		{
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 4; j++)
				{
					this->m[i][j] = other.m[i][j];
				}
			}
		}
		return *this;
	}

	inline friend std::ostream &operator<<(std::ostream &out, Matrix4 &matrix)
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				out << ' ' << matrix[i][j];
			}
			out << std::endl;
		}
		return out;
	}

	inline friend std::istream &operator>>(std::istream &in, Matrix4 &matrix)
	{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				in >> matrix[i][j];
			}
		}
		return in;
	}

	inline float *operator[](int row)
	{
		return this->m[row];
	}

	inline Matrix4 operator+(Matrix4 other)
	{
		Matrix4 result;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				result[i][j] = m[i][j] + other.m[i][j];
			}
		}
		return result;
	}

	inline Matrix4 operator-(Matrix4 other)
	{
		Matrix4 result;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				result[i][j] = m[i][j] - other.m[i][j];
			}
		}
		return result;
	}

	inline Matrix4 operator*(float a)
	{
		Matrix4 result;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				result[i][j] = m[i][j] * a;
			}
		}
		return result;
	}

	inline Matrix4 operator*(Matrix4 other)
	{
		Matrix4 result;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				result[i][j] = m[i][0] * other[0][j] + m[i][1] * other[1][j] + m[i][2] * other[2][j] + m[i][3] * other[3][j];
			}
		}
		return result;
	}

	inline Vector4 operator*(Vector4 v)
	{
		Vector4 result;
		for(int i = 0; i < 4; i++)
		{
			result[i] = m[i][0] * v[0] + m[i][1] * v[1] + m[i][2] * v[2] + m[i][3] * v[3];
		}
		return result;
	}

	inline Matrix4 operator/(float a)
	{
		Matrix4 result;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				result[i][j] = m[i][j] / a;
			}
		}
		return result;
	}

	inline void operator*=(float a)
	{
		*this = *this * a;
	}

	inline void operator*=(Matrix4 &other)
	{
		*this = *this * other;
	}

	inline void operator/=(float a)
	{
		*this = *this / a;
	}

	inline Matrix4 Transposed()
	{
		Matrix4 result;
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				result[i][j] = m[j][i];
			}
		}
		return result;
	}

	inline static Matrix4 IdentityMatrix()
	{
		Matrix4 matrix;
		matrix[0][0] = 1.0f;
		matrix[1][1] = 1.0f;
		matrix[2][2] = 1.0f;
		matrix[3][3] = 1.0f;
		return matrix;
	}

	inline static Matrix4 RotationX(float angleRad)
	{
		Matrix4 matrix;
		matrix[0][0] = 1.0f;
		matrix[1][1] = cosf(angleRad);
		matrix[1][2] = -sinf(angleRad);
		matrix[2][1] = sinf(angleRad);
		matrix[2][2] = cosf(angleRad);
		matrix[3][3] = 1.0f;
		return matrix;
	}

	inline static Matrix4 RotationY(float angleRad)
	{
		Matrix4 matrix;
		matrix[0][0] = cosf(angleRad);
		matrix[0][2] = sinf(angleRad);
		matrix[2][0] = -sinf(angleRad);
		matrix[2][2] = cosf(angleRad);
		matrix[1][1] = 1.0f;
		matrix[3][3] = 1.0f;
		return matrix;
	}

	inline static Matrix4 RotationZ(float angleRad)
	{
		Matrix4 matrix;
		matrix[0][0] = cosf(angleRad);
		matrix[0][1] = -sinf(angleRad);
		matrix[1][0] = sinf(angleRad);
		matrix[1][1] = cosf(angleRad);
		matrix[2][2] = 1.0f;
		matrix[3][3] = 1.0f;
		return matrix;
	}

	inline float Determinant()
	{
		Matrix3 m1, m2, m3, m4;
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				m1[i][j] = m[i + 1][j + 1];
				m2[i][j] = m[i + 1][j == 0 ? j : j + 1];
				m3[i][j] = m[i + 1][j <= 1 ? j : j + 1];
				m4[i][j] = m[i + 1][j];
			}
		}
		return m[0][0] * m1.Determinant() - m[0][1] * m2.Determinant() + m[0][2] * m3.Determinant() - m[0][3] * m4.Determinant();
	}

	inline Matrix4 InversedMatrix()
	{
		Matrix4 cij;

		for(int r = 0; r < 4; r++)
		{
			for(int c = 0; c < 4; c++)
			{
				Matrix3 adj;
				for(int i = 0, ii = 0; i < 4; i++)
				{
					if(i != r)
					{
						for(int j = 0, jj = 0; j < 4; j++)
						{
							if(j != c)
							{
								adj[ii][jj++] = m[i][j];
							}
						}
						ii++;
					}
				}

				cij[r][c] = adj.Determinant() * (float)(pow(-1, r + c));
			}
		}

		return (cij.Transposed() * (1 / (*this).Determinant()));
	}

	inline static Matrix4 ProjectionMatrix(float n, float f, float fov, float aspectRatio)
	{
		Matrix4 matrix;
		matrix[0][0] = aspectRatio / tanf(fov * 0.5f / 180.0f * 3.14159f);
		matrix[1][1] = 1.0f / tanf(fov / 2.0f * 3.1415f / 180.0f);
		matrix[2][2] = f / (f - n);
		matrix[2][3] = 1.0f;
		matrix[3][2] = -f * n / (f - n);
		return matrix;
	}

	inline static Matrix4 TranslationMatrix(float x, float y, float z)
	{
		Matrix4 matrix = Matrix4::IdentityMatrix();
		matrix[0][3] = x;
		matrix[1][3] = y;
		matrix[2][3] = z;
		return matrix;
	}

	inline static Matrix4 PointAtMatrix(Vector4 camLocation, Vector4 camRight, Vector4 camUp, Vector4 camForward, Vector4 target)
	{
		/*Vector4 camForward = (target - camLocation).Normalize();
		Vector4	camUp = up - (camForward * (up * camForward)).Normalize();
		Vector4 camRight = camUp.CrossProduct(camForward).Normalize();*/

		Matrix4 matrix;
		matrix[0][0] = camRight[0];		matrix[0][1] = camUp[0];	matrix[0][2] = camForward[0];	matrix[0][3] = target[0];
		matrix[1][0] = camRight[1];		matrix[1][1] = camUp[1];	matrix[1][2] = camForward[1];	matrix[1][3] = target[1];
		matrix[2][0] = camRight[2];		matrix[2][1] = camUp[2];	matrix[2][2] = camForward[2];	matrix[2][3] = target[2];
		matrix[3][0] = 0;				matrix[3][1] = 0;			matrix[3][2] = 0;				matrix[3][3] = 1;
		return matrix;
	}

	inline static Matrix4 LookAtMatrix(Vector4 camLocation, Vector4 camRight, Vector4 camUp, Vector4 camForward, Vector4 target)
	{
		Matrix4 pointAt = PointAtMatrix(camLocation, camRight, camUp, camForward, target);
		return pointAt.InversedMatrix();
	}

	inline static Matrix4 TransformationMatrix(Vector4 e1, Vector4 e2, Vector4 e3)
	{
		Matrix4 matrix;
		matrix[0][0] = e1[0]; matrix[0][1] = e2[0]; matrix[0][2] = e3[0]; matrix[0][3] = 0;
		matrix[1][0] = e1[1]; matrix[1][1] = e2[1]; matrix[1][2] = e3[1]; matrix[1][3] = 0;
		matrix[2][0] = e1[2]; matrix[2][1] = e2[2]; matrix[2][2] = e3[2]; matrix[2][3] = 0;
		matrix[3][0] = 0;	  matrix[3][1] = 0;		matrix[3][2] = 0;	  matrix[3][3] = 1;
		return matrix;
	}

	inline static Vector4 RotateAroundX(Vector4 e1, Vector4 e2, Vector4 e3, Vector4 center, Vector4 point, float theta)
	{
		Matrix4 T = Matrix4::TransformationMatrix(e1, e2, e3);
		Matrix4 A = Matrix4::RotationX(theta);

		Matrix4 matr = T * A * T.InversedMatrix();

		Vector4 result = matr * point + (center - matr * center);

		return result;
	}

	inline static Vector4 RotateAroundY(Vector4 e1, Vector4 e2, Vector4 e3, Vector4 center, Vector4 point, float theta)
	{
		Matrix4 T = Matrix4::TransformationMatrix(e1, e2, e3);
		Matrix4 A = Matrix4::RotationY(theta);

		Matrix4 matr = T * A * T.InversedMatrix();

		Vector4 result = matr * point + (center - matr * center);

		return result;
	}

	inline static Vector4 RotateAroundZ(Vector4 e1, Vector4 e2, Vector4 e3, Vector4 center, Vector4 point, float theta)
	{
		Matrix4 T = Matrix4::TransformationMatrix(e1, e2, e3);
		Matrix4 A = Matrix4::RotationZ(theta);

		Matrix4 matr = T * A * T.InversedMatrix();

		Vector4 result = matr * point + (center - matr * center);
		return result;
	}

	inline void SwapRows(float row1[], float row2[])
	{
		for(int i = 0; i < 4; i++)
		{
			float temp = row1[i];
			row1[i] = row2[i];
			row2[i] = temp;
		}
	}

	float m[4][4];
};

struct Vertex
{
	Vertex() : Vertex(0.0f, 0.0f, 0.0f)
	{}

	Vertex(float x, float y, float z)
	{
		this->pos = {x,y,z};
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
		}
		return *this;
	}

	Vector3 pos;
};

struct Triangle
{
	Triangle() : Triangle({Vertex(),Vertex(),Vertex()})
	{}

	Triangle(std::vector<Vertex> p)
	{
		for(int i = 0; i < 3; i++)
		{
			this->p[i] = p[i];
		}
	}

	Triangle(const Triangle &other)
	{
		*this = other;
	}

	Triangle &operator=(const Triangle &other)
	{
		if(this != &other)
		{
			for(int i = 0; i < 3; i++)
			{
				this->p[i] = other.p[i];
			}
		}
		return *this;
	}

	Vertex p[3];
};

struct Mesh
{
	Mesh() : Mesh(std::vector<Triangle>())
	{}

	Mesh(std::vector<Triangle> m)
	{
		this->m = m;
	}

	Mesh(const Mesh &other)
	{
		*this = other;
	}

	Mesh &operator=(const Mesh &other)
	{
		if(this != &other)
		{
			this->m = m;
		}
		return *this;
	}

	std::vector<Triangle> m;
};