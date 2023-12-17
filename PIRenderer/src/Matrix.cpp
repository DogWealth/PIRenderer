#include "Matrix.h"
#include <cmath>

#define PI 3.1415

namespace PIRenderer {
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
		Vector3f v(x, y, z);
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


	Vector3f operator*(const Vector3f& v, const Matrix4& m)
	{
		float x, y, z, w;

		y = v.x * m.m_Mat[0][1] + v.y * m.m_Mat[1][1] + v.z * m.m_Mat[2][1] + m.m_Mat[3][1];
		z = v.x * m.m_Mat[0][2] + v.y * m.m_Mat[1][2] + v.z * m.m_Mat[2][2] + m.m_Mat[3][2];
		w = v.x * m.m_Mat[0][3] + v.y * m.m_Mat[1][3] + v.z * m.m_Mat[2][3] + m.m_Mat[3][3];
		x = v.x * m.m_Mat[0][0] + v.y * m.m_Mat[1][0] + v.z * m.m_Mat[2][0] + m.m_Mat[3][0];

		return Vector3f(x / w, y / w, z / w);
	}

}


