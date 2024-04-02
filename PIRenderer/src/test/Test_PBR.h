#pragma once
#include "../Application.h"

namespace PIRenderer {
	class Test_PBR : public Application
	{
	public:
		Test_PBR();
		~Test_PBR();
		virtual void OnUpdate(double tick) override;

	protected:
		void ShowSphereMatrix();

	protected:
		Mesh* m_SphereMesh;
		Shader* m_SphereShader;

		Mesh* m_LightMesh;
		Shader* m_LightShader;

		Texture* m_AlbedoMap;
		Texture* m_NormalMap;
		Texture* m_MetallicMap;
		Texture* m_RoughnessMap;
		Texture* m_AoMap;
	};
}
