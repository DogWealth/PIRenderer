#pragma once
#include "PbrShader.h"

namespace PIRenderer {
	class IBLShader : public PbrShader
	{
	public:
		virtual void FragmentShader(V2F* v) override;
		void SetIrradianceMap(CubeMap* map) { m_IrradianceMap = map; }
		void SetPrefilterMap(CubeMap* map) { m_PrefilterMap = map; }
		void SetLUTMap(Texture* lut) { m_LUTMap = lut; }

	private:
		Vector3f FresnelSchlick(float cosTheta, Vector3f F0, float roughness);
		Vector3f FresnelSchlick(float cosTheta, Vector3f F0);
	private:
		CubeMap* m_IrradianceMap;
		CubeMap* m_PrefilterMap;
		Texture* m_LUTMap;
	};
}

