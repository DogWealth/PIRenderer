#pragma once
#include "Vector.h"
#include "Matrix.h"
namespace PIRenderer {
	class DirectionLight
	{
	public:
		DirectionLight();
		DirectionLight(const Vector3f& dir, const Vector3f& pos, float intensity);
		~DirectionLight(){}

		float GetIntensity() const { return m_Intensity; }
		const Vector3f GetPosition() const { return m_Position; }
		const Vector3f GetDirection() const { return -m_Direction; }

		Matrix4 LookAt(const Vector3f& up);
	private:
		float m_Intensity;
		Vector3f m_Position;
		Vector3f m_Direction;
	};
}

