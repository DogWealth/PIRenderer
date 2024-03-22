#pragma once
#include "../Application.h"
namespace PIRenderer {
	class Test_Blinn_Phong : public Application
	{
	public:
		Test_Blinn_Phong();
		virtual ~Test_Blinn_Phong();
		virtual void OnUpdate(double tick) override;

	private:
		Mesh* m_FloorMesh;
		Mesh* m_HeadMesh;
		Mesh* m_LightMesh;

		Shader* m_FloorShader;
		Shader* m_HeadShader;
		Shader* m_LightShader;
		Shader* m_SimpleDepthShader;

		Texture* m_HeadTexture;
		Texture* m_NormalMap;

		std::vector<std::vector<double>>* m_ExSAT;
		std::vector<std::vector<double>>* m_ExSquareSAT;

		float* m_DepthSquareBuffer;
		float* m_ShadowMap;
	};
}

