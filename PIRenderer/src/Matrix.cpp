#include "Matrix.h"
#include <cmath>

#define PI 3.1415

namespace PIRenderer {
	Matrix4::Matrix4()
	{
	}

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

	Matrix4 Matrix4::Transpose(const Matrix4& m)
	{
		Matrix4 mat4;
		mat4.m_Mat[0][0] = m.m_Mat[0][0];
		mat4.m_Mat[0][1] = m.m_Mat[1][0];
		mat4.m_Mat[0][2] = m.m_Mat[2][0];
		mat4.m_Mat[0][3] = m.m_Mat[3][0];

		mat4.m_Mat[1][0] = m.m_Mat[0][1];
		mat4.m_Mat[1][1] = m.m_Mat[1][1];
		mat4.m_Mat[1][2] = m.m_Mat[2][1];
		mat4.m_Mat[1][3] = m.m_Mat[3][1];

		mat4.m_Mat[2][0] = m.m_Mat[0][2];
		mat4.m_Mat[2][1] = m.m_Mat[1][2];
		mat4.m_Mat[2][2] = m.m_Mat[2][2];
		mat4.m_Mat[2][3] = m.m_Mat[3][2];

		mat4.m_Mat[3][0] = m.m_Mat[0][3];
		mat4.m_Mat[3][1] = m.m_Mat[1][3];
		mat4.m_Mat[3][2] = m.m_Mat[2][3];
		mat4.m_Mat[3][3] = m.m_Mat[3][3];

		return mat4;
	}

	Matrix4 Matrix4::Translate(float x, float y, float z)
	{
		Matrix4 m = Matrix4::Identity();
		m.m_Mat[3][0] = x;
		m.m_Mat[3][1] = y;
		m.m_Mat[3][2] = z;
		return m;
	}

	Matrix4 Matrix4::Translate(const Vector3f& pos)
	{
		return Translate(pos.x, pos.y, pos.z);
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
		
		float w = cos(PI * theta / (2 * 180.0f));
		x = v.x * sin(PI * theta / (2 * 180.0f));
		y = v.y * sin(PI * theta / (2 * 180.0f));
		z = v.z * sin(PI * theta / (2 * 180.0f));

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

	Matrix4 Matrix4::LookAt(const Vector3f& eyePos, const Vector3f& lookAt, const Vector3f& upAxis)
	{
		Vector3f lookDir = eyePos - lookAt;
		lookDir.Normalize();

		Vector3f rightDir = Vector3f::CrossProduct(upAxis, lookDir);
		rightDir.Normalize();

		Vector3f upDir = Vector3f::CrossProduct(lookDir, rightDir);
		upDir.Normalize();

		Matrix4 mat4;
		mat4.m_Mat[0][0] = rightDir.x;
		mat4.m_Mat[0][1] = upDir.x;
		mat4.m_Mat[0][2] = lookDir.x;
		mat4.m_Mat[0][3] = 0;

		mat4.m_Mat[1][0] = rightDir.y;
		mat4.m_Mat[1][1] = upDir.y;
		mat4.m_Mat[1][2] = lookDir.y;
		mat4.m_Mat[1][3] = 0;

		mat4.m_Mat[2][0] = rightDir.z;
		mat4.m_Mat[2][1] = upDir.z;
		mat4.m_Mat[2][2] = lookDir.z;
		mat4.m_Mat[2][3] = 0;

		mat4.m_Mat[3][0] = 0;
		mat4.m_Mat[3][1] = 0;
		mat4.m_Mat[3][2] = 0;
		mat4.m_Mat[3][3] = 1;

		//这里犯错了，记住，多推几遍，注意upAxis是世界的轴，不是摄像机的，updir才是摄像机的y方向
		return Matrix4::Translate(-eyePos.x, -eyePos.y, -eyePos.z) * mat4;
	}

	Matrix4 Matrix4::Orthographic(float l, float r, float b, float t, float f, float n)
	{
		Matrix4 mat4;
		mat4.m_Mat[0][0] = 2 / (r - l);
		mat4.m_Mat[0][1] = 0;
		mat4.m_Mat[0][2] = 0;
		mat4.m_Mat[0][3] = 0;

		mat4.m_Mat[1][0] = 0;
		mat4.m_Mat[1][1] = 2 / (t - b);
		mat4.m_Mat[1][2] = 0;
		mat4.m_Mat[1][3] = 0;

		mat4.m_Mat[2][0] = 0;
		mat4.m_Mat[2][1] = 0;
		mat4.m_Mat[2][2] = 2 / (n - f);
		mat4.m_Mat[2][3] = 0;

		mat4.m_Mat[3][0] = -(r + l) / (r - l);
		mat4.m_Mat[3][1] = -(t + b) / (t - b);
		mat4.m_Mat[3][2] = (n + f) / (n - f);
		mat4.m_Mat[3][3] = 1;

		return mat4;
	}

	Matrix4 Matrix4::Perspective(float n, float f, float fov, float aspectRatio)
	{
		float t = tan(PI * fov / (2 * 180)) * n;
		float b = -t;
		float r = t * aspectRatio;
		float l = -r;

		Matrix4 mat4;
		mat4.m_Mat[0][0] = n;
		mat4.m_Mat[0][1] = 0;
		mat4.m_Mat[0][2] = 0;
		mat4.m_Mat[0][3] = 0;

		mat4.m_Mat[1][0] = 0;
		mat4.m_Mat[1][1] = n;
		mat4.m_Mat[1][2] = 0;
		mat4.m_Mat[1][3] = 0;

		mat4.m_Mat[2][0] = 0;
		mat4.m_Mat[2][1] = 0;
		mat4.m_Mat[2][2] = f + n;
		mat4.m_Mat[2][3] = -1;

		mat4.m_Mat[3][0] = 0;
		mat4.m_Mat[3][1] = 0;
		mat4.m_Mat[3][2] = f * n;
		mat4.m_Mat[3][3] = 0;

		mat4 =  mat4 * Matrix4::Orthographic(l, r, b, t, f, n);

		return mat4;
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

		x = v.x * m.m_Mat[0][0] + v.y * m.m_Mat[1][0] + v.z * m.m_Mat[2][0] + m.m_Mat[3][0];
		y = v.x * m.m_Mat[0][1] + v.y * m.m_Mat[1][1] + v.z * m.m_Mat[2][1] + m.m_Mat[3][1];
		z = v.x * m.m_Mat[0][2] + v.y * m.m_Mat[1][2] + v.z * m.m_Mat[2][2] + m.m_Mat[3][2];
		w = v.x * m.m_Mat[0][3] + v.y * m.m_Mat[1][3] + v.z * m.m_Mat[2][3] + m.m_Mat[3][3];

		return Vector3f(x, y, z, w);
	}

}


