#include "BasicShader.h"
#include "../Vector.h"
#include "stdio.h"
namespace PIRenderer {
	BasicShader::BasicShader()
	{
		m_Texture = new Texture("obj/african_head_diffuse.tga");
	}

	BasicShader::~BasicShader()
	{
		delete m_Texture;
	}

	void BasicShader::VertexShader(Vertex* v1, Vertex* v2, Vertex* v3)
	{

	}

	void BasicShader::FragmentShader(Vertex* v)
	{
		Vector3f n = v->m_Normal;
		n.Normalize();

		float intensity = m_DirectionLight.GetIntensity();
		Vector3f dir = m_DirectionLight.GetDirection();

		intensity = n * dir;

		intensity = intensity > 0 ? intensity : 0.f;

		Vector3f color = m_Texture->Sample(v->m_TexCoord.u, v->m_TexCoord.v);

		v->m_Color = color * intensity;
	}

}