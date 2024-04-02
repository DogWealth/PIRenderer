#pragma once
#include "BasicShader.h"
#include "../math.h"
namespace PIRenderer {
	class PbrShader : public BasicShader
	{
	public:
		virtual void FragmentShader(V2F* v) override;

		void SetAlbedo(const Vector3f& albedo) { m_albedo = albedo; }
		void SetMetallic(float metallic) { m_metallic = metallic; }
		void SetRoughness(float roughness) { m_roughness = roughness; }
		void SetAmbientOcclusion(float ao) { m_ao = ao; }

		void SetAlbedoMap(Texture* albedo) { m_AlbedoMap = albedo; }
		void SetMetallicMap(Texture* MetallicMap) { m_MetallicMap = MetallicMap; }
		void SetRoughnessMap(Texture* RoughnessMap) { m_RoughnessMap = RoughnessMap; }
		void SetAoMap(Texture* AoMap) { m_AoMap = AoMap; }

	protected:
		Vector3f FresnelSchlick(float cosTheta, Vector3f F0);
		float DistributionGGX(Vector3f N, Vector3f H, float roughness);
		float GeometrySchlickGGX(float NdotV, float roughness);
		float GeometrySmith(Vector3f N, Vector3f V, Vector3f L, float roughness);

	protected:
		Vector3f m_albedo;//∑¥…‰¬ 
		float m_metallic = 0.0f; 
		float m_roughness;
		float m_ao;

		Texture* m_AlbedoMap;
		Texture* m_MetallicMap;
		Texture* m_RoughnessMap;
		Texture* m_AoMap;

	};

}

