#include "BasicShader.h"
#include "../Vector.h"
#include "stdio.h"
namespace PIRenderer {
	BasicShader::BasicShader()
	{
		m_Texture = nullptr;
	}

	BasicShader::~BasicShader()
	{
	}

	void BasicShader::VertexShader(Vertex* v1, Vertex* v2, Vertex* v3)
	{
		v1->m_Position = v1->m_Position * m_VPMatrix;
		v2->m_Position = v2->m_Position * m_VPMatrix;
		v3->m_Position = v3->m_Position * m_VPMatrix;

		v1->m_Position.x /= v1->m_Position.w;
		v2->m_Position.x /= v2->m_Position.w;
		v3->m_Position.x /= v3->m_Position.w;

		v1->m_Position.y /= v1->m_Position.w;
		v2->m_Position.y /= v2->m_Position.w;
		v3->m_Position.y /= v3->m_Position.w;

		v1->m_Position.z = -v1->m_Position.w;
		v2->m_Position.z = -v2->m_Position.w;
		v3->m_Position.z = -v3->m_Position.w;

		v1->m_Position.w = 1.0f;
		v2->m_Position.w = 1.0f;
		v3->m_Position.w = 1.0f;

		Vertex_rhw_Init(v1);
		Vertex_rhw_Init(v2);
		Vertex_rhw_Init(v3);
	}

	void BasicShader::FragmentShader(Vertex* v)
	{

		v->m_Normal.Normalize();

		float intensity = v->m_Normal * m_DirectionLight.GetDirection();

		if (intensity > 0)
		{
			v->m_Color = *(m_Texture->Sample(v->m_TexCoord.u, v->m_TexCoord.v)) * intensity;
		}
		else
		{
			v->m_Color = { 0, 0, 0 };
		}
	}

	void BasicShader::SetTexture(Texture* texture)
	{
		m_Texture = texture;
	}

	void BasicShader::SetVPMatrix(Matrix4 vpMatrix)
	{
		m_VPMatrix = vpMatrix;
	}

	void BasicShader::Vertex_rhw_Init(Vertex* v)
	{
		float rhw_z = 1.0f / v->m_Position.z;

		v->m_Position.z = rhw_z;
		v->m_Color *= rhw_z;
		v->m_Normal *= rhw_z;
		v->m_TexCoord *= rhw_z;
	}

}