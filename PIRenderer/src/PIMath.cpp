#include "PIMath.h"
#include <cmath>

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
}