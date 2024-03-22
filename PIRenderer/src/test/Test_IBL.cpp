#include "Test_IBL.h"
#include "../shader/SkyBoxShader.h"
PIRenderer::Test_IBL::Test_IBL()
{
	Texture* posx = new PIRenderer::Texture("obj/cubemap/SkyBox/posx.tga");
	Texture* posy = new PIRenderer::Texture("obj/cubemap/SkyBox/posy.tga");
	Texture* posz = new PIRenderer::Texture("obj/cubemap/SkyBox/posz.tga");
	Texture* negx = new PIRenderer::Texture("obj/cubemap/SkyBox/negx.tga");
	Texture* negy = new PIRenderer::Texture("obj/cubemap/SkyBox/negy.tga");
	Texture* negz = new PIRenderer::Texture("obj/cubemap/SkyBox/negz.tga");

	m_CubeMap = new PIRenderer::CubeMap(posx, posy, posz, negx, negy, negz);
	m_SkyBox = new PIRenderer::Mesh("obj/cube.obj");
	m_SkyBoxShader = PIRenderer::Shader::Create("SkyBoxShader");

	dynamic_cast<SkyBoxShader*>(m_SkyBoxShader)->SetCubeMap(m_CubeMap);

	m_Renderer->SetDepthSquareBuffer(nullptr);
	m_Renderer->SetDepthBuffer(m_DepthBuffer);
}

PIRenderer::Test_IBL::~Test_IBL()
{
	m_CubeMap->DelteAllTextures();
	delete m_CubeMap;
	delete m_SkyBoxShader;
}

void PIRenderer::Test_IBL::OnUpdate(double tick)
{
	m_Controller->OnUpdate(tick);


	m_Renderer->Clear();

	//SkyBox
	Matrix4 SkyVMatrix = m_Controller->GetCamera().GetViewMatrix();
	Matrix4 SkyPMatrix = m_Controller->GetCamera().GetProjectionMatrix();

	SkyVMatrix.m_Mat[3][0] = 0;
	SkyVMatrix.m_Mat[3][1] = 0;
	SkyVMatrix.m_Mat[3][2] = 0;

	m_Renderer->BindShader(m_SkyBoxShader);
	m_SkyBoxShader->SetVPMatrix(SkyVMatrix * SkyPMatrix);

	m_Renderer->UseBackFaceCulling(false);
	m_Renderer->UseDepthTest(false);

	m_Renderer->DrawMesh(m_SkyBox);


	m_Window->OnUpdate();
}
