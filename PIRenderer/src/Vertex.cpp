#include "Vertex.h"

namespace PIRenderer {
	Vertex::Vertex(const Vector3f& pos, const Vector3f& normal, const Vector2& texcoord, const Vector3f& color)
		: m_Position(pos), m_Normal(normal), m_TexCoord(texcoord), m_Color(color)
	{
	}

	Vertex Vertex::Interpolate(const Vertex& v1, const Vertex& v2, float t)
	{
		Vector3f pos = Vector3f::Interpolate(v1.m_Position, v2.m_Position, t);
		Vector3f normal = Vector3f::Interpolate(v1.m_Normal, v2.m_Normal, t);
		Vector3f color = Vector3f::Interpolate(v1.m_Color, v2.m_Color, t);
		Vector2 texcoord = Vector2::Interpolate(v1.m_TexCoord, v2.m_TexCoord, t);

		return Vertex(pos, normal, texcoord, color);
	}

	Vertex* Vertex::Interpolate(Vertex* v, const Vertex& v1, const Vertex& v2, float t)
	{
		Vector3f::Interpolate(&v->m_Position, 	v1.m_Position,	v2.m_Position, t);
		Vector3f::Interpolate(&v->m_Normal,		v1.m_Normal,	v2.m_Normal, t);
		Vector3f::Interpolate(&v->m_Color,		v1.m_Color,		v2.m_Color, t);
		Vector2::Interpolate(&v->m_TexCoord,	v1.m_TexCoord,	v2.m_TexCoord, t);

		return v;
	}

}