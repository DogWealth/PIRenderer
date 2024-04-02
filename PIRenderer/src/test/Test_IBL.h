#pragma once
#include "../Application.h"
#include "Test_PBR.h"
namespace PIRenderer {
	class Test_IBL : public Test_PBR
	{
	public:
		Test_IBL();
		virtual ~Test_IBL();
		virtual void OnUpdate(double tick) override;

	private:
		Mesh* m_SkyBox;
		Shader* m_SkyBoxShader;
		CubeMap* m_CubeMap;
		CubeMap* m_IrradianceCubeMap;
		CubeMap* m_PrefilterCubeMap;
		Texture* m_BrdfLUTMap;
	};
}

