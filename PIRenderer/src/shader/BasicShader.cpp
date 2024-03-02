#include "BasicShader.h"
#include "../Vector.h"
#include "stdio.h"
namespace PIRenderer {
	BasicShader::BasicShader()
	{
		m_Texture = nullptr;
		m_ModelMatrix = Matrix4::Identity();
	}

	BasicShader::~BasicShader()
	{
	}

	V2F BasicShader::VertexShader(const Vertex& v)
	{
		//VertexShader中不应该执行透视除法，透视除法是一个单独的过程，VertexShader输出的是四维齐次坐标
		V2F v2f;
		v2f.m_WorldPos	= v.m_Position * m_ModelMatrix;
		v2f.m_ScreenPos = v.m_Position * m_ModelMatrix * m_VPMatrix;
		v2f.m_Normal	= v.m_Normal;
		v2f.m_Texcoord	= v.m_TexCoord;
		v2f.m_Color		= v.m_Color;

		Vertex_rhw_Init(&v2f);

		return v2f;
	}

	void BasicShader::FragmentShader(V2F* v)
	{

		v->m_Normal.Normalize();

		float intensity = v->m_Normal * m_DirectionLight.GetDirection();

		if (intensity > 0)
		{
			if (m_Texture)
				v->m_Color = *(m_Texture->Sample(v->m_Texcoord.u, v->m_Texcoord.v)) * intensity;
			else
				v->m_Color = { 1, 1, 1 };
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

	void BasicShader::SetModelMatrix(Matrix4 Model)
	{
		m_ModelMatrix = Model;
	}

	void BasicShader::Vertex_rhw_Init(V2F* v2f)
	{
		v2f->m_rhw = 1.0f / -v2f->m_ScreenPos.w;

		v2f->m_WorldPos *= v2f->m_rhw;
		v2f->m_Color	*= v2f->m_rhw;
		v2f->m_Normal	*= v2f->m_rhw;
		v2f->m_Texcoord *= v2f->m_rhw;
	}

}