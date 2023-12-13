#include "Vertex.h"

namespace PIRenderer {
	Vertex::Vertex(const Vector3& pos, const Vector3& normal, float u, float v, const Vector3 color)
		: m_Position(pos), m_Normal(normal), u(u), v(v), m_Color(color)
	{
	}

	Vertex Vertex::Interpolate(const Vertex& v1, const Vertex& v2, float t)
	{
		Vector3 pos = Vector3::Interpolate(v1.m_Position, v2.m_Position, t);
		Vector3 normal = Vector3::Interpolate(v1.m_Normal, v2.m_Normal, t);
		Vector3 color = Vector3::Interpolate(v1.m_Color, v2.m_Color, t);

		float u = v1.u + (v2.u - v1.u) * t;
		float v = v1.v + (v2.v - v1.v) * t;

		return Vertex(pos, normal, u, v, color);
	}

}