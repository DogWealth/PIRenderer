#include "PbrShader.h"

namespace PIRenderer {
	void PbrShader::FragmentShader(V2F* v)
	{
		if (m_AlbedoMap)
		{
			m_albedo = (m_AlbedoMap->Sample(v->m_Texcoord.u, v->m_Texcoord.v));
			m_albedo = Vector3f::Pow(m_albedo, 2.2f);
		}
		if (m_MetallicMap)	m_metallic = (m_MetallicMap->Sample(v->m_Texcoord.u, v->m_Texcoord.v)).z;
		if (m_RoughnessMap) m_roughness = (m_RoughnessMap->Sample(v->m_Texcoord.u, v->m_Texcoord.v)).z;
		if (m_AoMap)		m_ao = (m_AoMap->Sample(v->m_Texcoord.u, v->m_Texcoord.v)).z;

		Vector3f N = Vector3f::Normalize(v->m_Normal);
		if (m_NormalMap)
		{
			Vector3f tangent = Vector3f::Normalize(m_Tangent - (m_Tangent * N) * N);
			Vector3f bitangent = Vector3f::CrossProduct(tangent, N);
			Matrix4 TBN = GetTBN(tangent, bitangent, N);
			N = (m_NormalMap->Sample(v->m_Texcoord.u, v->m_Texcoord.v));
			N = (N * 2.0f + -1.0f);
			N.Normalize();
			N = N * TBN;
		}
		Vector3f V = Vector3f::Normalize(m_eyePos - v->m_WorldPos);

		Vector3f F0 = { 0.04f, 0.04f, 0.04f };
		F0 = Vector3f::Interpolate(F0, m_albedo, m_metallic);

		Vector3f Lo = { 0.0, 0.0, 0.0 };

		Vector3f L = Vector3f::Normalize(m_DirectionLight.GetPosition() - v->m_WorldPos);
		Vector3f H = Vector3f::Normalize(V + L);
		float distance = (m_DirectionLight.GetPosition() - v->m_WorldPos).Mag();
		float attenuation = 1.0 / (distance * distance);
		float intensity = m_DirectionLight.GetIntensity();
		Vector3f lightColor = { intensity, intensity, intensity };
		Vector3f radiance = lightColor * attenuation;

		// cook-torrance brdf
		float NDF = DistributionGGX(N, H, m_roughness);
		float G = GeometrySmith(N, V, L, m_roughness);
		Vector3f F = FresnelSchlick(std::max(H * V, 0.0f), F0);

		Vector3f kS = F;
		Vector3f kD = Vector3f(1.0f, 1.0f, 1.0f) - kS;
		kD *= (1.0 - m_metallic);

		Vector3f nominator = NDF * G * F;
		float denominator = 4.0 * std::max(N * V, 0.0f) * std::max(N * L, 0.0f) + 0.001;
		Vector3f specular = nominator / denominator;

		// add to outgoing radiance Lo
		float NdotL = std::max(N * L, 0.0f);

		Lo = Vector3f::Mul((specular + Vector3f::Mul(kD, m_albedo) / PI), radiance) * NdotL;

		Vector3f ambient = Vector3f::Mul(Vector3f(0.03f, 0.03f, 0.03f), m_albedo) * m_ao;
		Vector3f color = Lo + ambient;

		color = color / (color + 1.0f);
		color = Vector3f::Pow(color, (1.0 / 2.2));

		v->m_Color = color;

	}

	Vector3f PbrShader::FresnelSchlick(float cosTheta, Vector3f F0)
	{
		return F0 + (-F0 + 1.0) * pow(std::min(std::max(1.0 - cosTheta, 0.0), 1.0), 5.0);
	}

	float PbrShader::DistributionGGX(Vector3f N, Vector3f H, float roughness)
	{
		float a = roughness * roughness;
		float a2 = a * a;
		float NdotH = std::max(N * H, 0.0f);
		float NdotH2 = NdotH * NdotH;

		float num = a2;
		float denom = (NdotH2 * (a2 - 1.0) + 1.0);
		denom = PI * denom * denom;

		return num / denom;
	}
	float PbrShader::GeometrySchlickGGX(float NdotV, float roughness)
	{
		float r = (roughness + 1.0);
		float k = (r * r) / 8.0;

		float num = NdotV;
		float denom = NdotV * (1.0 - k) + k;

		return num / denom;
	}
	float PbrShader::GeometrySmith(Vector3f N, Vector3f V, Vector3f L, float roughness)
	{
		float NdotV = std::max(N * V, 0.0f);
		float NdotL = std::max(N * L, 0.0f);
		float ggx2 = GeometrySchlickGGX(NdotV, roughness);
		float ggx1 = GeometrySchlickGGX(NdotL, roughness);

		return ggx1 * ggx2;
	}
}
