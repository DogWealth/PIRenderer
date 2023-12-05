#pragma once
namespace PIRenderer {
	class Vector3
	{
	public:
		Vector3() {};
		Vector3(float x, float y, float z);
		Vector3(const Vector3& vector);

		~Vector3() = default;

		Vector3& operator=(const Vector3& vector);
		bool operator==(const Vector3& vector) const;
		bool operator!=(const Vector3& vector) const;

		Vector3 operator-() const;

		Vector3 operator+(const Vector3& vector) const;
		Vector3 operator-(const Vector3& vector) const;
		Vector3& operator+=(const Vector3& vector);
		Vector3& operator-=(const Vector3& vector);

		//标量右乘
		Vector3 operator*(float k) const;
		Vector3 operator/(float k) const;
		Vector3& operator*=(float k);
		Vector3& operator/=(float k);

		float operator*(const Vector3& vector) const;

		void Zero();
		void Normalize();
		float Mag(); //求模长

		//向量求模
		static float VectorMag(const Vector3& vector);

		//叉乘
		static Vector3 CrossProduct(const Vector3& v1, const Vector3& v2);

		//计算两点距离
		static float Distance(const Vector3& v1, const Vector3& v2);


	public:
		float x;
		float y;
		float z;

		static Vector3 ZeroVector;
	};

	//标量左乘 非成员函数
	inline Vector3 operator*(float k, const Vector3& vector);
}

