#include "Blinn_PhongShader.h"
#include <algorithm>

void PIRenderer::Blinn_PhongShader::FragmentShader(V2F* v)
{
	Vector3f objectColor;
	if (m_Texture)
		objectColor = *(m_Texture->Sample(v->m_Texcoord.u, v->m_Texcoord.v));
	else
		objectColor = { 1, 1, 1 };
	float lightIntensity = m_DirectionLight.GetIntensity();

	//������
	float Ka = 0.2;
	float ambient = lightIntensity * Ka;

	//������
	Vector3f normal = Vector3f::Normalize(v->m_Normal);
	Vector3f lightDir = m_DirectionLight.GetDirection();
	float Kd = 0.7;
	float diff = std::max(normal * lightDir, 0.0f) * Kd;
	float diffuse = lightIntensity * diff;

	//���淴��
	float Ks = 0.5;
	Vector3f half = Vector3f::Normalize((lightDir - m_EyePos));
	float spec = std::pow(std::max(normal * half, 0.0f), 32) * Ks;
	float specular = lightIntensity * spec;


	v->m_Color = objectColor * (ambient + diffuse + specular);
}

void PIRenderer::Blinn_PhongShader::SetEyePos(const Vector3f& eyepos)
{
	m_EyePos = eyepos;
}
