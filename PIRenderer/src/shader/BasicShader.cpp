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
		v->m_Normal.Normalize();

		float intensity = m_DirectionLight.GetIntensity();

		intensity = v->m_Normal * m_DirectionLight.GetDirection();

		intensity = intensity > 0 ? intensity : 0.f;

		v->m_Color = m_Texture->Sample(v->m_TexCoord.u, v->m_TexCoord.v) * intensity;
	}

}