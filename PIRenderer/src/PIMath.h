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

		//�����ҳ�
		Vector3 operator*(float k) const;
		Vector3 operator/(float k) const;
		Vector3& operator*=(float k);
		Vector3& operator/=(float k);

		float operator*(const Vector3& vector) const;

		void Zero();
		void Normalize();
		float Mag(); //��ģ��

		//������ģ
		static float VectorMag(const Vector3& vector);

		//���
		static Vector3 CrossProduct(const Vector3& v1, const Vector3& v2);

		//�����������
		static float Distance(const Vector3& v1, const Vector3& v2);


	public:
		float x;
		float y;
		float z;

		static Vector3 ZeroVector;
	};

	//������� �ǳ�Ա����
	inline Vector3 operator*(float k, const Vector3& vector);

	/***********Matrix*********/
	class Matrix4
	{
	public:
		Matrix4() {}
		Matrix4(const Matrix4& m);

		Matrix4& operator=(const Matrix4& m);

		static Matrix4 Identity();
		static Matrix4 Zero();

		static Matrix4 Translate(float x, float y, float z);

		static Matrix4 Scale(float x, float y, float z);

		static Matrix4 RotateX(float pitch);
		static Matrix4 RotateY(float yaw);
		static Matrix4 RotateZ(float roll);
		static Matrix4 RotateEuler(float roll, float pitch, float yaw);
		static Matrix4 Rotate(float x, float y, float z, float theta);
	public:
		float m_Mat[4][4];
	};


	//�ǳ�Ա����
	Matrix4 operator+(const Matrix4& m1, const Matrix4& m2);
	Matrix4& operator+=(Matrix4& m1, const Matrix4& m2);

	Matrix4 operator-(const Matrix4& m1, const Matrix4& m2);
	Matrix4& operator-=(Matrix4& m1, const Matrix4& m2);

	Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);

	Vector3 operator*(const Vector3& v, const Matrix4& m); //��inline��linkerror
}

