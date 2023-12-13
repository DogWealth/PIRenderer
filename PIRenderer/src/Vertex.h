#pragma once
#include "PIMath.h"

namespace PIRenderer {
	class Vertex
	{
	public:
		Vertex(const Vector3& pos, const Vector3& normal, float u, float v, const Vector3 color);

		static Vertex Interpolate(const Vertex& v1, const Vertex& v2, float t);

	public:
		Vector3 m_Position;
		Vector3 m_Normal;
		Vector3 m_Color;
		float u;
		float v;
	};
}

