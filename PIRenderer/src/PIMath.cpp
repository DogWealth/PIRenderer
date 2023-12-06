#include "PIMath.h"
#include <cmath>

#define PI 3.1415

namespace PIRenderer {
	/********Vector3*************/
	Vector3 Vector3::ZeroVector = Vector3(0.0f, 0.0f, 0.0f);

	Vector3::Vector3(float x, float y, float z)
		: x(x), y(y), z(z)
	{
	}

	Vector3::Vector3(const Vector3& vector)
	{
		this->x = vector.x;
		this->y = vector.y;
		this->z = vector.z;
	}

	Vector3& Vector3::operator=(const Vector3& vector)
	{
		this->x = vector.x;
		this->y = vector.y;
		this->z = vector.z;
		
		return *this;
	}

	bool Vector3::operator==(const Vector3& vector) const
	{
		return this->x == vector.x || this->y == vector.y || this->z == vector.z;
	}

	bool Vector3::operator!=(const Vector3& vector) const
	{
		return this->x != vector.x || this->y != vector.y || this->z != vector.z;
	}

	Vector3 Vector3::operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	Vector3 Vector3::operator+(const Vector3& vector) const
	{
		return Vector3(x + vector.x, y + vector.y, z + vector.z);
	}

	Vector3 Vector3::operator-(const Vector3& vector) const
	{
		return Vector3(x - vector.x, y - vector.y, z - vector.z);
	}

	Vector3& Vector3::operator+=(const Vector3& vector)
	{
		this->x += vector.x;
		this->y += vector.y;
		this->z += vector.z;

		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& vector)
	{
		this->x -= vector.x;
		this->y -= vector.y;
		this->z -= vector.z;

		return *this;
	}

	Vector3 Vector3::operator*(float k) const
	{
		return Vector3(x * k, y * k, z * k);
	}

	Vector3 Vector3::operator/(float k) const
	{
		return Vector3(x / k, y / k, z / k);
	}

	Vector3& Vector3::operator*=(float k)
	{
		this->x *= k;
		this->y *= k;
		this->z *= k;

		return *this;
	}

	Vector3& Vector3::operator/=(float k)
	{
		float rec = 1.0f / k;
		this->x *= rec;
		this->y *= rec;
		this->z *= rec;

		return *this;
	}

	float Vector3::operator*(const Vector3& vector) const
	{
		return x * vector.x + y * vector.y + z * vector.z;
	}

	void Vector3::Zero()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	void Vector3::Normalize()
	{
		float Magnitude = x * x + y * y + z * z;
		if (Magnitude > 0.0f) //¼ì²é³ý0
		{
			float OneOverMag = 1.0f / sqrt(Magnitude);
			x *= OneOverMag;
			y *= OneOverMag;
			z *= OneOverMag;
		}
	}

	float Vector3::Mag()
	{
		return sqrt(x * x + y * y + z * z);
	}

	float Vector3::VectorMag(const Vector3& vector)
	{
		return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	}

	Vector3 Vector3::CrossProduct(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(v1.y * v2.z - v1.z * v2.y,
						v1.x * v2.z - v1.z * v2.x,
						v1.x * v2.y - v1.y * v2.z);
	}

	float Vector3::Distance(const Vector3& v1, const Vector3& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		float dz = v1.z - v2.z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}


	Vector3 operator*(float k, const Vector3& vector)
	{
		return Vector3(vector.x * k, vector.y * k, vector.z * k);
	}


	/****************Matrix******************/

	Matrix4::Matrix4(const Matrix4& m)
	{
		for(int i=0; i<4; i++)
			for (int j = 0; j < 4; j++)
			{
				m_Mat[i][j] = m.m_Mat[i][j];
			}
	}

	Matrix4& Matrix4::operator=(const Matrix4& m)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				m_Mat[i][j] = m.m_Mat[i][j];
			}

		return *this;
	}

	Matrix4 Matrix4::Identity()
	{
		Matrix4 m = Matrix4::Zero();
		for (int i = 0; i < 4; i++)
			m.m_Mat[i][i] = 1.0f;
		return m;
	}

	Matrix4 Matrix4::Zero()
	{
		Matrix4 m;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				m.m_Mat[i][j] = 0.0f;
			}
		return m;
	}

	Matrix4 Matrix4::Translate(float x, float y, float z)
	{
		Matrix4 m = Matrix4::Identity();
		m.m_Mat[3][0] = x;
		m.m_Mat[3][1] = y;
		m.m_Mat[3][2] = z;
		return m;
	}

	Matrix4 Matrix4::Scale(float x, float y, float z)
	{
		Matrix4 m = Matrix4::Identity();
		m.m_Mat[0][0] = x;
		m.m_Mat[1][1] = y;
		m.m_Mat[2][2] = z;
		return m;
	}

	Matrix4 Matrix4::RotateX(float pitch)
	{
		Matrix4 m = Matrix4::Identity();
		float theta = PI * pitch / 180.0f;
		m.m_Mat[1][1] = cos(theta);
		m.m_Mat[1][2] = -sin(theta);
		m.m_Mat[2][1] = sin(theta);
		m.m_Mat[2][2] = cos(theta);

		return m;
	}

	Matrix4 Matrix4::RotateY(float roll)
	{
		Matrix4 m = Matrix4::Identity();
		float theta = PI * roll / 180.0f;
		m.m_Mat[0][0] = cos(theta);
		m.m_Mat[0][2] = sin(theta);
		m.m_Mat[2][0] = -sin(theta);
		m.m_Mat[2][2] = cos(theta);

		return m;
	}

	Matrix4 Matrix4::RotateZ(float yaw)
	{
		Matrix4 m = Matrix4::Identity();
		float theta = PI * yaw / 180.0f;
		m.m_Mat[0][0] = cos(theta);
		m.m_Mat[0][1] = -sin(theta);
		m.m_Mat[1][0] = sin(theta);
		m.m_Mat[1][1] = cos(theta);

		return m;
	}

	Matrix4 Matrix4::RotateEuler(float roll, float pitch, float yaw)
	{
		return RotateY(roll) * RotateX(pitch) * RotateZ(yaw);
	}

	Matrix4 Matrix4::Rotate(float x, float y, float z, float theta)
	{
		Matrix4 m = Matrix4::Identity();
		Vector3 v(x, y, z);
		v.Normalize();
		
		float w = cos(PI * theta / 2 * 180.0f);
		x = v.x * sin(PI * theta / 2 * 180.0f);
		y = v.y * sin(PI * theta / 2 * 180.0f);
		z = v.z * sin(PI * theta / 2 * 180.0f);

		m.m_Mat[0][0] = 1 - 2 * y * y - 2 * z * z;
		m.m_Mat[1][0] = 2 * x * y - 2 * w * z;
		m.m_Mat[2][0] = 2 * x * z + 2 * w * y;
		m.m_Mat[0][1] = 2 * x * y + 2 * w * z;
		m.m_Mat[1][1] = 1 - 2 * x * x - 2 * z * z;
		m.m_Mat[2][1] = 2 * y * z - 2 * w * x;
		m.m_Mat[0][2] = 2 * x * z - 2 * w * y;
		m.m_Mat[1][2] = 2 * y * z + 2 * w * x;
		m.m_Mat[2][2] = 1 - 2 * x * x - 2 * y * y;

		return m;
	}

	Matrix4 operator+(const Matrix4& m1, const Matrix4& m2)
	{
		Matrix4 m;
		for(int i=0; i<4; i++)
			for (int j = 0; j < 4; j++)
			{
				m.m_Mat[i][j] = m1.m_Mat[i][j] + m2.m_Mat[i][j];
			}
		return m;
	}

	Matrix4& operator+=(Matrix4& m1, const Matrix4& m2)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				m1.m_Mat[i][j] += m2.m_Mat[i][j];
			}

		return m1;
	}

	Matrix4 operator-(const Matrix4& m1, const Matrix4& m2)
	{
		Matrix4 m;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				m.m_Mat[i][j] = m1.m_Mat[i][j] - m2.m_Mat[i][j];
			}

		return m;
	}

	Matrix4& operator-=(Matrix4& m1, const Matrix4& m2)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				m1.m_Mat[i][j] -= m2.m_Mat[i][j];
			}

		return m1;
	}

	Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
	{
		Matrix4 m;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				m.m_Mat[i][j] = (
					m1.m_Mat[i][0] * m2.m_Mat[0][j] +
					m1.m_Mat[i][1] * m2.m_Mat[1][j] +
					m1.m_Mat[i][2] * m2.m_Mat[2][j] +
					m1.m_Mat[i][3] * m2.m_Mat[3][j]
					);
			}

		return m;
	}


	Vector3 operator*(const Vector3& v, const Matrix4& m)
	{
		float x, y, z, w;

		y = v.x * m.m_Mat[0][1] + v.y * m.m_Mat[1][1] + v.z * m.m_Mat[2][1] + m.m_Mat[3][1];
		z = v.x * m.m_Mat[0][2] + v.y * m.m_Mat[1][2] + v.z * m.m_Mat[2][2] + m.m_Mat[3][2];
		w = v.x * m.m_Mat[0][3] + v.y * m.m_Mat[1][3] + v.z * m.m_Mat[2][3] + m.m_Mat[3][3];
		x = v.x * m.m_Mat[0][0] + v.y * m.m_Mat[1][0] + v.z * m.m_Mat[2][0] + m.m_Mat[3][0];

		return Vector3(x / w, y / w, z / w);
	}

}