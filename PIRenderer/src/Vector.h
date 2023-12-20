#pragma once
#include <cmath>
namespace PIRenderer {
	template<class T>
	class Vector3
	{
	public:
		Vector3<T>() { x = y = z = 0; }
		Vector3(T x, T y, T z);
		Vector3(const Vector3<T>& vector);

		~Vector3() = default;

		Vector3<T>& operator=(const Vector3<T>& vector);
		bool operator==(const Vector3<T>& vector) const;
		bool operator!=(const Vector3<T>& vector) const;

		Vector3<T> operator-() const;

		Vector3<T> operator+(const Vector3<T>& vector) const;
		Vector3<T> operator-(const Vector3<T>& vector) const;
		Vector3<T>& operator+=(const Vector3<T>& vector);
		Vector3<T>& operator-=(const Vector3<T>& vector);

		//�����ҳ�
		Vector3<T> operator*(float k) const;
		Vector3<T> operator/(float k) const;
		Vector3<T>& operator*=(float k);
		Vector3<T>& operator/=(float k);

		T operator*(const Vector3<T>& vector) const;

		void Zero();
		void Normalize();
		float Mag(); //��ģ��

		//������ģ
		static float VectorMag(const Vector3<T>& vector);

		//���
		static Vector3<T> CrossProduct(const Vector3<T>& v1, const Vector3<T>& v2);

		//�����������
		static float Distance(const Vector3<T>& v1, const Vector3<T>& v2);

		//��ֵ
		static Vector3<T> Interpolate(const Vector3<T>& v1, const Vector3<T>& v2, float t);
		static Vector3<T>* Interpolate(Vector3<T>* v, const Vector3<T>& v1, const Vector3<T>& v2, float t);


	public:
		T x;
		T y;
		T z;

		static Vector3<T> ZeroVector;
	};

	//������� �ǳ�Ա����
	template<class T>
	inline Vector3<T> operator*(float k, const Vector3<T>& vector);

	using Vector3f = Vector3<float>;
	using Vector3i = Vector3<int>;

	template<class T>
	Vector3<T> Vector3<T>::ZeroVector = Vector3<T>(0, 0, 0);

	template<class T>
	Vector3<T>::Vector3(T x, T y, T z)
		: x(x), y(y), z(z)
	{
	}

	template<class T>
	Vector3<T>::Vector3(const Vector3<T>& vector)
	{
		this->x = vector.x;
		this->y = vector.y;
		this->z = vector.z;
	}

	template<class T>
	Vector3<T>& Vector3<T>::operator=(const Vector3<T>& vector)
	{
		this->x = vector.x;
		this->y = vector.y;
		this->z = vector.z;

		return *this;
	}

	template<class T>
	bool Vector3<T>::operator==(const Vector3<T>& vector) const
	{
		return this->x == vector.x || this->y == vector.y || this->z == vector.z;
	}

	template<class T>
	bool Vector3<T>::operator!=(const Vector3<T>& vector) const
	{
		return this->x != vector.x || this->y != vector.y || this->z != vector.z;
	}

	template<class T>
	Vector3<T> Vector3<T>::operator-() const
	{
		return Vector3<T>(-x, -y, -z);
	}

	template<class T>
	Vector3<T> Vector3<T>::operator+(const Vector3<T>& vector) const
	{
		return Vector3<T>(x + vector.x, y + vector.y, z + vector.z);
	}

	template<class T>
	Vector3<T> Vector3<T>::operator-(const Vector3<T>& vector) const
	{
		return Vector3<T>(x - vector.x, y - vector.y, z - vector.z);
	}

	template<class T>
	Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& vector)
	{
		this->x += vector.x;
		this->y += vector.y;
		this->z += vector.z;

		return *this;
	}

	template<class T>
	Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& vector)
	{
		this->x -= vector.x;
		this->y -= vector.y;
		this->z -= vector.z;

		return *this;
	}

	template<class T>
	Vector3<T> Vector3<T>::operator*(float k) const
	{
		return Vector3<T>(x * k, y * k, z * k);
	}

	template<class T>
	Vector3<T> Vector3<T>::operator/(float k) const
	{
		return Vector3<T>(x / k, y / k, z / k);
	}

	template<class T>
	Vector3<T>& Vector3<T>::operator*=(float k)
	{
		this->x *= k;
		this->y *= k;
		this->z *= k;

		return *this;
	}

	template<class T>
	Vector3<T>& Vector3<T>::operator/=(float k)
	{
		float rec = 1.0f / k;
		this->x *= rec;
		this->y *= rec;
		this->z *= rec;

		return *this;
	}

	template<class T>
	T Vector3<T>::operator*(const Vector3<T>& vector) const
	{
		return x * vector.x + y * vector.y + z * vector.z;
	}

	template<class T>
	void Vector3<T>::Zero()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	template<class T>
	void Vector3<T>::Normalize()
	{
		float Magnitude = x * x + y * y + z * z;
		if (Magnitude > 0.0f) //����0
		{
			float OneOverMag = 1.0f / sqrt(Magnitude);
			x *= OneOverMag;
			y *= OneOverMag;
			z *= OneOverMag;
		}
	}

	template<class T>
	float Vector3<T>::Mag()
	{
		return sqrt(x * x + y * y + z * z);
	}

	template<class T>
	float Vector3<T>::VectorMag(const Vector3<T>& vector)
	{
		return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	}

	template<class T>
	Vector3<T> Vector3<T>::CrossProduct(const Vector3<T>& v1, const Vector3<T>& v2) //��������ϵ
	{
		return Vector3(v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
	}

	template<class T>
	float Vector3<T>::Distance(const Vector3<T>& v1, const Vector3<T>& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		float dz = v1.z - v2.z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	template<class T>
	inline Vector3<T> Vector3<T>::Interpolate(const Vector3<T>& v1, const Vector3<T>& v2, float t)
	{

		return v1 + (v2 - v1) * t;
	}

	template<class T>
	inline Vector3<T>* Vector3<T>::Interpolate(Vector3<T>* v, const Vector3<T>& v1, const Vector3<T>& v2, float t)
	{
		v->x = v1.x + (v2.x - v1.x) * t;
		v->y = v1.y + (v2.y - v1.y) * t;
		v->z = v1.z + (v2.z - v1.z) * t;
		return v;
	}

	template<class T>
	Vector3<T> operator*(float k, const Vector3<T>& vector)
	{
		return Vector3<T>(vector.x * k, vector.y * k, vector.z * k);
	}



	/*********Vector2**********/
	class Vector2
	{
	public:
		Vector2() { u = v = 0.f; }
		Vector2(float u, float v)
		{
			this->u = u;
			this->v = v;
		}

		Vector2 operator+(const Vector2& vec2) const
		{
			return Vector2(u + vec2.u, v + vec2.v);
		}

		Vector2 operator-(const Vector2& vec2) const
		{
			return Vector2(u - vec2.u, v - vec2.v);
		}

		Vector2 operator*(float k) const
		{
			return Vector2(u * k, v * k);
		}

		Vector2 operator/(float k) const
		{
			return Vector2(u / k, v / k);
		}

		static Vector2 Interpolate(const Vector2& v1, const Vector2& v2, float t)
		{
			return v1 + (v2 - v1) * t;
		}
		static Vector2* Interpolate(Vector2* v, const Vector2& v1, const Vector2& v2, float t)
		{
			v->u = v1.u + (v2.u - v1.u) * t;
			v->v = v1.v + (v2.v - v1.v) * t;
			
			return v;
		}


	public:
		float u, v;
	};

}
