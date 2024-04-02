#pragma once
#include "BasicShader.h"
namespace PIRenderer {
	class BrdfLUTShader : public BasicShader
	{
	public:
		virtual void FragmentShader(V2F* v) override;
		void SetLUTMap(Texture* texture) { m_BrdfLUT = texture; }

	private:
		Vector2 IntegrateBRDF(float NdotV, float roughness);
		float GeometrySchlickGGX(float NdotV, float roughness);
		float GeometrySmith(Vector3f N, Vector3f V, Vector3f L, float roughness);
	private:
		Texture* m_BrdfLUT;
	};
}

