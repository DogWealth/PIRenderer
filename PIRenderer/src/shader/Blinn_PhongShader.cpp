#include "Blinn_PhongShader.h"
#include <algorithm>

PIRenderer::Blinn_PhongShader::Blinn_PhongShader()
{
	m_ShadowMap = nullptr;
	m_UseShadow = false;
}

PIRenderer::Blinn_PhongShader::Blinn_PhongShader(float* shadowMap, int width, int height, bool useShadow = true)
	: m_ShadowMap(shadowMap), m_SMWidth(width), m_SMHeight(height), m_UseShadow(useShadow)
{
}

void PIRenderer::Blinn_PhongShader::FragmentShader(V2F* v)
{
	Vector3f objectColor;
	if (m_Texture)
		objectColor = *(m_Texture->Sample(v->m_Texcoord.u, v->m_Texcoord.v));
	else
		objectColor = { 1, 1, 1 };
	float lightIntensity = m_DirectionLight.GetIntensity();

	//环境光
	float Ka = 0.2;
	float ambient = lightIntensity * Ka;

	//漫反射
	Vector3f normal = Vector3f::Normalize(v->m_Normal);
	Vector3f lightDir = m_DirectionLight.GetDirection();
	float Kd = 0.7;
	float diff = std::max(normal * lightDir, 0.0f) * Kd;
	float diffuse = lightIntensity * diff;

	//镜面反射
	float Ks = 0.5;
	Vector3f half = Vector3f::Normalize((lightDir - m_EyePos));
	float spec = std::pow(std::max(normal * half, 0.0f), 32) * Ks;
	float specular = lightIntensity * spec;

	if (m_UseShadow && m_ShadowMap != nullptr)
	{
		float shadow = ShadowClaculation(v->m_WorldPos);
		v->m_Color = objectColor * (ambient + (1.0 - shadow) * diffuse + specular);
	}
	else
		v->m_Color = objectColor * (ambient + diffuse + specular);
}

void PIRenderer::Blinn_PhongShader::SetEyePos(const Vector3f& eyepos)
{
	m_EyePos = eyepos;
}

void PIRenderer::Blinn_PhongShader::SetShaowMap(float* shadowMap, int width, int height)
{
	m_ShadowMap = shadowMap;
	m_SMWidth = width;
	m_SMHeight = height;
}

void PIRenderer::Blinn_PhongShader::UseShadow(bool use)
{
	m_UseShadow = use;
}

void PIRenderer::Blinn_PhongShader::SetLightSpaceMatrix(const Matrix4& m)
{
	m_LightSpace = m;
}

float PIRenderer::Blinn_PhongShader::ShadowClaculation(const Vector3f& worldPos)
{
	Vector3f lightSpacePos = worldPos * m_LightSpace;
	lightSpacePos.x /= lightSpacePos.w;
	lightSpacePos.y /= lightSpacePos.w;
	lightSpacePos.z /= lightSpacePos.w;

	int x = (lightSpacePos.x + 1.0) * 0.5f * m_SMWidth;
	int y = (1.0 - lightSpacePos.y) * 0.5f * m_SMHeight;
	float closestDepth = m_ShadowMap[y * m_SMWidth + x];
	float currentDepth = lightSpacePos.z;

	return (currentDepth > closestDepth ? 1.0f : 0.0f);
}
