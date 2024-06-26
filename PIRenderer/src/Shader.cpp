#include "Shader.h"
#include "shader/BasicShader.h"
#include "shader/LightShader.h"
#include "shader/Blinn_PhongShader.h"
#include "shader/SimpleDepthShader.h"
#include "shader/SkyBoxShader.h"
#include "shader/PbrShader.h"
#include "shader/SimpleIrradianceShader.h"
#include "shader/IBLShader.h"
#include "shader/PrefilterShader.h"
#include "shader/BrdfLUTShader.h"
namespace PIRenderer {
	Shader* Shader::Create(const std::string& name)
	{
		if (name == "BasicShader")
			return new BasicShader();
		else if (name == "LightShader")
			return new LightShader();
		else if (name == "Blinn_PhongShader")
			return new Blinn_PhongShader();
		else if (name == "SimpleDepthShader")
			return new SimpleDepthShader();
		else if (name == "SkyBoxShader")
			return new SkyBoxShader();
		else if (name == "PbrShader")
			return new PbrShader();
		else if (name == "SimpleIrradianceShader")
			return new SimpleIrradianceShader();
		else if (name == "IBLShader")
			return new IBLShader();
		else if (name == "PrefilterShader")
			return new PrefilterShader();
		else if (name == "BrdfLUTShader")
			return new BrdfLUTShader();

		return nullptr;
	}
	V2F::V2F(const Vector3f& worldPos, const Vector3f& screenPos, const Vector3f& normal, const Vector3f& color, const Vector2& texture)
		: m_WorldPos(worldPos), m_ScreenPos(screenPos), m_Normal(normal), m_Color(color), m_Texcoord(texture), m_rhw(worldPos.z)
	{
	}

	V2F::V2F(const V2F& v)
	{
		this->m_WorldPos = v.m_WorldPos;
		this->m_ScreenPos = v.m_ScreenPos;
		this->m_Normal = v.m_Normal;
		this->m_Color = v.m_Color;
		this->m_Texcoord = v.m_Texcoord;
		this->m_rhw = v.m_rhw; //忘记写导致std::swap出现错误！！！！应该是拷贝出现问题，有时间研究下
	}

	V2F* V2F::Interpolate(V2F* v, const V2F& v1, const V2F& v2, float t)
	{
		Vector3f::Interpolate(&v->m_WorldPos, v1.m_WorldPos, v2.m_WorldPos, t);
		Vector3f::Interpolate(&v->m_ScreenPos, v1.m_ScreenPos, v2.m_ScreenPos, t);
		Vector3f::Interpolate(&v->m_Normal, v1.m_Normal, v2.m_Normal, t);
		Vector3f::Interpolate(&v->m_Color, v1.m_Color, v2.m_Color, t);
		Vector2::Interpolate(&v->m_Texcoord, v1.m_Texcoord, v2.m_Texcoord, t);
		v->m_rhw = v1.m_rhw + (v2.m_rhw - v1.m_rhw) * t;

		return v;
	}
}