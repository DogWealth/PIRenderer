#include "Test_Blinn_Phong.h"
#include "../shader/Blinn_PhongShader.h"
#include "../math.h"
PIRenderer::Test_Blinn_Phong::Test_Blinn_Phong()
{
	m_FloorMesh = new PIRenderer::Mesh("obj/floor.obj");//注意相对路径时相对PIRenderer这个文件夹开始的
	m_FloorShader = PIRenderer::Shader::Create("Blinn_PhongShader");

	m_HeadMesh = new PIRenderer::Mesh("obj/african_head.obj");
	m_HeadShader = PIRenderer::Shader::Create("Blinn_PhongShader");
	m_HeadTexture = new PIRenderer::Texture("obj/african_head_diffuse.tga");
	//m_NormalMap = new Texture("obj/african_normal_tangent.tga");

	m_LightMesh = new PIRenderer::Mesh("obj/cube.obj");
	m_LightShader = PIRenderer::Shader::Create("LightShader");

	m_SimpleDepthShader = PIRenderer::Shader::Create("SimpleDepthShader");

	m_HeadShader->SetTexture(m_HeadTexture);
	m_HeadShader->SetNormalMap(m_NormalMap);

	//vssm
	m_ExSAT = new std::vector<std::vector<double>>(HEIGHT, std::vector<double>(WIDTH, 0));
	m_ExSquareSAT = new std::vector<std::vector<double>>(HEIGHT, std::vector<double>(WIDTH, 0));

	m_DepthSquareBuffer = new float[WIDTH * HEIGHT];
	m_ShadowMap = new float[WIDTH * HEIGHT];
}

PIRenderer::Test_Blinn_Phong::~Test_Blinn_Phong()
{
	delete m_FloorMesh;
	delete m_FloorShader;
	delete m_HeadMesh;
	delete m_HeadShader;
	delete m_HeadTexture;
	delete m_LightMesh;
	delete m_LightShader;
	delete m_SimpleDepthShader;
	delete m_ExSAT;
	delete m_ExSquareSAT;
	delete[] m_ShadowMap;
	delete[] m_DepthSquareBuffer;
}

void PIRenderer::Test_Blinn_Phong::OnUpdate(double tick)
{
	m_Controller->OnUpdate(tick);
	Matrix4 VPMatrix = m_Controller->GetCamera().GetViewProjectionMatrix();
	Matrix4 ModelMatrix = Matrix4::Scale(0.1, 0.1, 0.1) * Matrix4::RotateX(90) * Matrix4::Translate(0, -1, 0);
	Matrix4 HeadModelMatrix = Matrix4::Scale(1, 1, 1);

	Vector3f lightPos = { (float)(2 * sin(clock() / 10 * PI / 180.0f) * cos(45 * PI / 180.0f)),
							(float)(2 * sin(45 * PI / 180.0f)),
							(float)(2 * cos(clock() / 10 * PI / 180.0f) * cos(45 * PI / 180.0f)) };


	DirectionLight dLight = { -lightPos, lightPos, 1 };

	Matrix4 LightModelMatrix = Matrix4::Scale(0.1, 0.1, 0.1) * Matrix4::Translate(dLight.GetPosition());

	Matrix4 LightSpaceMatrix = Matrix4::LookAt(lightPos, lightPos, { 0, 1, 0 }) *
		Matrix4::Orthographic(-5, 5, -5, 5, 50, 1);

	//caculate shadowmap
	//m_Renderer->SetDepthBuffer(m_ShadowMap);
	//m_Renderer->SetDepthSquareBuffer(m_DepthSquareBuffer);//这个每次也要绑定跟新
	//m_Renderer->UseDepthTest(true);
	//m_Renderer->Clear();

	//m_Renderer->BindShader(m_SimpleDepthShader);
	//m_SimpleDepthShader->SetVPMatrix(LightSpaceMatrix);
	//m_SimpleDepthShader->SetModelMatrix(HeadModelMatrix);
	//Render(m_HeadMesh);

	//m_SimpleDepthShader->SetModelMatrix(ModelMatrix);
	//Render(m_FloorMesh);


	//GetSummedAreaTable(m_ExSAT, m_ShadowMap, WIDTH, HEIGHT);
	//GetSummedAreaTable(m_ExSquareSAT, m_DepthSquareBuffer, WIDTH, HEIGHT);


	dynamic_cast<Blinn_PhongShader*>(m_FloorShader)->SetExSAT(m_ExSAT);
	dynamic_cast<Blinn_PhongShader*>(m_FloorShader)->SetExSquareSAT(m_ExSquareSAT);

	dynamic_cast<Blinn_PhongShader*>(m_FloorShader)->SetShaowMap(m_ShadowMap, WIDTH, HEIGHT);
	dynamic_cast<Blinn_PhongShader*>(m_FloorShader)->UseShadow(false);
	dynamic_cast<Blinn_PhongShader*>(m_FloorShader)->UsePCF(false);
	dynamic_cast<Blinn_PhongShader*>(m_FloorShader)->UsePCSS(false);
	dynamic_cast<Blinn_PhongShader*>(m_FloorShader)->UseVSSM(false);
	dynamic_cast<Blinn_PhongShader*>(m_FloorShader)->SetLightSpaceMatrix(LightSpaceMatrix);

	dynamic_cast<Blinn_PhongShader*>(m_HeadShader)->SetShaowMap(m_ShadowMap, WIDTH, HEIGHT);
	dynamic_cast<Blinn_PhongShader*>(m_HeadShader)->UseShadow(false);
	dynamic_cast<Blinn_PhongShader*>(m_HeadShader)->SetLightSpaceMatrix(LightSpaceMatrix);

	m_Renderer->SetDepthSquareBuffer(nullptr);
	m_Renderer->SetDepthBuffer(m_DepthBuffer);
	m_Renderer->Clear();

	//common render
	m_Renderer->UseBackFaceCulling(true);
	m_Renderer->UseDepthTest(true);

	m_FloorShader->SetLight(dLight);
	m_HeadShader->SetLight(dLight);

	m_Renderer->BindShader(m_FloorShader);
	m_FloorShader->SetVPMatrix(VPMatrix);
	m_FloorShader->SetModelMatrix(ModelMatrix);
	m_FloorShader->SetEyePos(m_Controller->GetCamera().GetPosition());

	Render(m_FloorMesh);

	m_Renderer->BindShader(m_HeadShader);
	m_HeadShader->SetVPMatrix(VPMatrix);
	m_HeadShader->SetModelMatrix(HeadModelMatrix);
	m_HeadShader->SetEyePos(m_Controller->GetCamera().GetPosition());

	Render(m_HeadMesh);


	//light
	m_Renderer->BindShader(m_LightShader);
	m_LightShader->SetVPMatrix(VPMatrix);
	m_LightShader->SetModelMatrix(LightModelMatrix);

	Render(m_LightMesh);

	m_Window->OnUpdate();
}
