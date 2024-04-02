#include "PrefilterShader.h"
#include "../math.h"
namespace PIRenderer {
	void PrefilterShader::FragmentShader(V2F* v)
	{
		Vector3f N = Vector3f::Normalize(v->m_WorldPos);
		Vector3f R = N;
		Vector3f V = R;

		const uint32_t SampleCount = 1024;
		float totalWeight = 0.0f;
		Vector3f prefilterColor = { 0, 0, 0 };

		for (uint32_t i = 0; i < SampleCount; i++)
		{
			Vector2 Xi = Hammersley(i, SampleCount);
			Vector3f H = ImportanceSampleGGX(Xi, N, m_Roughness);
			Vector3f L = Vector3f::Normalize(2.0 * (V * H) * H - V);

			float NdotL = N * L;
			if (NdotL > 0.0f)
			{
				float D = DistributionGGX(N * H, m_Roughness);
				float pdf = (D * (N * H) / (4.0 * (H * V))) + 0.0001;

				float resolution = m_CubeMap->GetWidth();
				float saTexel = 4.0 * PI / (6.0 * resolution * resolution);
				float saSample = 1.0 / (float(SampleCount) * pdf + 0.0001);
				float mipLevel = m_Roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

				prefilterColor += m_CubeMap->TrilinearSample(L, mipLevel) * NdotL;
				totalWeight += NdotL;
			}
		}
		
		prefilterColor /= totalWeight;

		int face = m_CubeMap->GetFace(N);
		
		Vector2 uv = m_CubeMap->GetUV(face, N);
		m_PrifilterMap->Set(uv.u, uv.v, prefilterColor);
	}

	float PrefilterShader::DistributionGGX(float NdotH, float roughness)
	{
		float a = roughness * roughness;
		float a2 = a * a;
		float NdotH2 = NdotH * NdotH;

		float num = a2;
		float denom = (NdotH2 * (a2 - 1.0) + 1.0);
		denom = PI * denom * denom;

		return num / denom;
	}
}
