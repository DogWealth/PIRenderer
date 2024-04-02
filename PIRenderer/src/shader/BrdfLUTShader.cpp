#include "BrdfLUTShader.h"
#include "../math.h"
namespace PIRenderer {
	void BrdfLUTShader::FragmentShader(V2F* v)
	{
		Vector2 integratedBRDF = IntegrateBRDF(v->m_Texcoord.u, v->m_Texcoord.v);
		Vector3f color = {integratedBRDF.u, integratedBRDF.v, 0.0f};
		m_BrdfLUT->Set(v->m_Texcoord.u, v->m_Texcoord.v, color);
	}

	Vector2 BrdfLUTShader::IntegrateBRDF(float NdotV, float roughness)
	{
		Vector3f V;
		V.x = (1.0 - NdotV * NdotV);
		V.y = 0;
		V.z = NdotV;

		float A = 0.0f;
		float B = 0.0f;

		Vector3f N = { 0, 0, 1 };

		const unsigned int SampleCount = 1024;
		for (unsigned int i = 0; i < SampleCount; i++)
		{
			Vector2 Xi = Hammersley(i, SampleCount);
			Vector3f H = ImportanceSampleGGX(Xi, N, roughness);
			Vector3f L = Vector3f::Normalize(2.0 * V * H * H - V);

			float NdotL = N * L;
			float NdotH = N * H;
			float VdotH = V * H;

			if (NdotL > 0)
			{
				float G = GeometrySmith(N, V, L, roughness);
				float G_Vis = (G * VdotH) / (NdotH * NdotV);
				float Fc = pow(1.0 - VdotH, 5.0);

				A += (1.0 - Fc) * G_Vis;
				B += Fc * G_Vis;
			}
		}

		A /= (float)SampleCount;
		B /= (float)SampleCount;

		return Vector2(A, B);
	}

	float BrdfLUTShader::GeometrySchlickGGX(float NdotV, float roughness)
	{
		float a = roughness;
		float k = (a * a) / 2.0;

		float nom = NdotV;
		float denom = NdotV * (1.0 - k) + k;

		return nom / denom;
	}

	float BrdfLUTShader::GeometrySmith(Vector3f N, Vector3f V, Vector3f L, float roughness)
	{
		float NdotV = std::max(N * V, 0.0f);
		float NdotL = std::max(N * L, 0.0f);
		float ggx2 = GeometrySchlickGGX(NdotV, roughness);
		float ggx1 = GeometrySchlickGGX(NdotL, roughness);

		return ggx1 * ggx2;
	}
}