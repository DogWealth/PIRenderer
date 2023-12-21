#include "Light.h"
#include "stdio.h"
#include "Matrix.h"
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
		return Matrix4::LookAt(m_Position, m_Direction, upAxis);
	}

}