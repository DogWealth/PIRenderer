#include "IBLShader.h"

namespace PIRenderer {
	void IBLShader::FragmentShader(V2F* v)
	{
		if (m_IrradianceMap == nullptr)
		{
			printf("Ã»ÓÐÉèÖÃIrradianceMap!\n");
			return;
		}
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

		//ambient
		kS = FresnelSchlick(std::max(N * V, 0.0f), F0, m_roughness);
		kD = -kS + 1.0f;
		kD *= (1.0 - m_metallic);
		Vector3f irradiance = m_IrradianceMap->Sample(N);
		Vector3f diffuse = Vector3f::Mul(irradiance, m_albedo);

		const float MAX_REFLECTION_LOD = 4.0;
		Vector3f R = Vector3f::Normalize(2.0 * V * N * N - V);
		Vector3f prefliterColor = m_PrefilterMap->
			TrilinearSample(R, m_roughness * MAX_REFLECTION_LOD);
		Vector3f envBrdf = m_LUTMap->Sample(std::max(N * V, 0.0f), m_roughness);
		F = FresnelSchlick(std::max(N * V, 0.0f), F0, m_roughness);
		specular = Vector3f::Mul(prefliterColor, (F * envBrdf.x + envBrdf.y));
		//specular = Vector3f::Mul(specular, m_albedo);

		Vector3f ambient = (Vector3f::Mul(diffuse, kD) + specular) * m_ao;

		Vector3f color = ambient + Lo;

		color = color / (color + 1.0f);
		color = Vector3f::Pow(color, (1.0 / 2.2));

		v->m_Color = color;
	}

	Vector3f IBLShader::FresnelSchlick(float cosTheta, Vector3f F0, float roughness)
	{
		return F0 + (Max(Vector3f(1.0f - roughness), F0) - F0) * pow(std::min(std::max(1.0 - cosTheta, 0.0), 1.0), 5.0);
	}

	Vector3f IBLShader::FresnelSchlick(float cosTheta, Vector3f F0)
	{
		return F0 + (-F0 + 1.0) * pow(std::min(std::max(1.0 - cosTheta, 0.0), 1.0), 5.0);
	}
}
