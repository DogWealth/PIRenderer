#include "Light.h"
#include "stdio.h"
namespace PIRenderer {
	DirectionLight::DirectionLight()
		:m_Direction({ 0, 0, -1 }), m_Position({0, 0, 0}), m_Intensity(1)
	{
		printf("未初始化光源，使用默认光源!\n");
	}
	DirectionLight::DirectionLight(const Vector3f& dir, const Vector3f& pos, float intensity)
		: m_Direction(dir), m_Position(pos), m_Intensity(intensity)
	{
	}

	Matrix4 DirectionLight::LookAt(const Vector3f& upAxis)
	{
		Vector3f lookDir = m_Direction;
		lookDir.Normalize();

		Vector3f rightDir = Vector3f::CrossProduct(upAxis, lookDir);
		rightDir.Normalize();

		Vector3f upDir = Vector3f::CrossProduct(lookDir, rightDir);
		upDir.Normalize();

		Matrix4 mat4;
		mat4.m_Mat[0][0] = rightDir.x;
		mat4.m_Mat[0][1] = rightDir.y;
		mat4.m_Mat[0][2] = rightDir.z;
		mat4.m_Mat[0][3] = -m_Position.x;

		mat4.m_Mat[1][0] = upDir.x;
		mat4.m_Mat[1][1] = upDir.y;
		mat4.m_Mat[1][2] = upDir.z;
		mat4.m_Mat[1][3] = -m_Position.y;

		mat4.m_Mat[2][0] = upDir.x;
		mat4.m_Mat[2][1] = upDir.y;
		mat4.m_Mat[2][2] = upDir.z;
		mat4.m_Mat[2][3] = -m_Position.z;

		mat4.m_Mat[3][0] = 0;
		mat4.m_Mat[3][1] = 0;
		mat4.m_Mat[3][2] = 0;
		mat4.m_Mat[3][3] = 1;
		
		return mat4;
	}

}