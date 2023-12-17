#pragma once
#include "Matrix.h"

namespace PIRenderer {
	class Vertex
	{
	public:
		Vertex(const Vector3f& pos, const Vector3f& normal, const Vector2& texcoord, const Vector3f& color = {1, 1, 1});

		static Vertex Interpolate(const Vertex& v1, const Vertex& v2, float t);

	public:
		Vector3f m_Position;
		Vector3f m_Normal;
		Vector3f m_Color;
		Vector2 m_TexCoord;
	};
}
