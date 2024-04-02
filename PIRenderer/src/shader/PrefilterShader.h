#pragma once
#include "SimpleIrradianceShader.h"
namespace PIRenderer {
	class PrefilterShader : public SimpleIrradianceShader
	{
	public:
		virtual void FragmentShader(V2F* v) override;
		void SetPrifilterMap(Texture* texture) { m_PrifilterMap = texture; }
		void SetRoughness(float r) { m_Roughness = r; }

	private:
		float DistributionGGX(float NdotH, float roughness);
	private:
		Texture* m_PrifilterMap;
		float m_Roughness;
	};
}

