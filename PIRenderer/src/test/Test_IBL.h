#pragma once
#include "../Application.h"
namespace PIRenderer {
	class Test_IBL : public Application
	{
	public:
		Test_IBL();
		virtual ~Test_IBL();
		virtual void OnUpdate(double tick) override;

	private:
		Mesh* m_SkyBox;
		Shader* m_SkyBoxShader;
		CubeMap* m_CubeMap;
	};
}

