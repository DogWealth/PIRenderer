#pragma once
#include "Vector.h"
namespace PIRenderer {
	class Matrix4
	{
	public:
		Matrix4() {}
		Matrix4(const Matrix4& m);

		Matrix4& operator=(const Matrix4& m);

		static Matrix4 Identity();
		static Matrix4 Zero();
		static Matrix4 Transpose(const Matrix4& m);

		static Matrix4 Translate(float x, float y, float z);
		static Matrix4 Translate(const Vector3f& pos);

		static Matrix4 Scale(float x, float y, float z);

		static Matrix4 RotateX(float pitch);
		static Matrix4 RotateY(float yaw);
		static Matrix4 RotateZ(float roll);
		static Matrix4 RotateEuler(float roll, float pitch, float yaw);
		static Matrix4 Rotate(float x, float y, float z, float theta);

		static Matrix4 LookAt(const Vector3f& eyePos, const Vector3f& lookAt, const Vector3f& upAxis);
		static Matrix4 Orthographic(float l, float r, float b, float t, float f, float n);
		static Matrix4 Perspective(float n, float f, float fov, float aspectRatio);
	public:
		float m_Mat[4][4];
	};


	//非成员函数
	Matrix4 operator+(const Matrix4& m1, const Matrix4& m2);
	Matrix4& operator+=(Matrix4& m1, const Matrix4& m2);

	Matrix4 operator-(const Matrix4& m1, const Matrix4& m2);
	Matrix4& operator-=(Matrix4& m1, const Matrix4& m2);

	Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);

	Vector3f operator*(const Vector3f& v, const Matrix4& m); //加inline会linkerror
}

