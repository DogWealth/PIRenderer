#include "Test_IBL.h"
#include "../shader/SkyBoxShader.h"
#include "../shader/SimpleIrradianceShader.h"
#include "../shader/IBLShader.h"
#include "../shader/PrefilterShader.h"
#include "../shader/BrdfLUTShader.h"
#include "../math.h"
#include <format>
#define SCENE Indoor
#define GENERATEIRRADIANCEMAP false
#define GENERATEPREFLITERMAP false
#define GENERATELUTMAP false
PIRenderer::Test_IBL::Test_IBL()
{
	enum Scene
	{
		CornellBox, GraceCathedral, Indoor, SkyBox
	};

	int textureSize[4] = { 32, 32, 32, 64 };//indirect
	int resolution[4] = { 128, 256, 512, 512 };

	Scene scene = SCENE;
	int ts = textureSize[scene];
	int rs = resolution[scene];

	std::vector<std::string> pathes = {
		"obj/cubemap/CornellBox/",
		"obj/cubemap/GraceCathedral/",
		"obj/cubemap/Indoor/",
		"obj/cubemap/SkyBox/",
	};

	std::vector<std::string> filesname = {
		"posx.tga",
		"posy.tga",
		"posz.tga",
		"negx.tga",
		"negy.tga",
		"negz.tga"
	};

	std::vector<Texture> cubemaps(6);

	std::string path = pathes[scene];

	for (int i = 0; i < 6; i++)
	{
		cubemaps[i] = Texture(path + filesname[i]);
	}

	m_CubeMap = 
		new CubeMap(cubemaps[0], cubemaps[1], cubemaps[2], cubemaps[3], cubemaps[4], cubemaps[5]);
	m_SkyBox = new Mesh("obj/cube.obj");
	m_SkyBoxShader = Shader::Create("SkyBoxShader");

	dynamic_cast<SkyBoxShader*>(m_SkyBoxShader)->SetCubeMap(m_CubeMap);

	m_Renderer->SetDepthSquareBuffer(nullptr);
	m_Renderer->SetDepthBuffer(m_DepthBuffer);

	//precompute
	Matrix4 captureProjection = Matrix4::Perspective(0.1f, 10.f, 90.f, 1.0f);
	std::vector<Matrix4> captureViews = {
		Matrix4::LookAt(Vector3f(0.f, 0.f, 0.f), Vector3f(1.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f)),
		Matrix4::LookAt(Vector3f(0.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f), Vector3f(0.f, 0.f, 1.f)),
		Matrix4::LookAt(Vector3f(0.f, 0.f, 0.f), Vector3f(0.f, 0.f, 1.f), Vector3f(0.f, 1.f, 0.f)),
		Matrix4::LookAt(Vector3f(0.f, 0.f, 0.f), Vector3f(-1.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f)),
		Matrix4::LookAt(Vector3f(0.f, 0.f, 0.f), Vector3f(0.f, -1.f, 0.f), Vector3f(0.f, 0.f, 1.f)),
		Matrix4::LookAt(Vector3f(0.f, 0.f, 0.f), Vector3f(0.f, 0.f, -1.f), Vector3f(0.f, 1.f, 0.f))
	};

	std::vector<Texture> irrandianceMaps = {
		Texture(ts, ts),
		Texture(ts, ts),
		Texture(ts, ts),
		Texture(ts, ts),
		Texture(ts, ts),
		Texture(ts, ts)
	};

	m_Renderer->UseBackFaceCulling(false);
	m_Renderer->UseDepthTest(false);

	//irradiance map
	if (GENERATEIRRADIANCEMAP)
	{
		Shader* irrandianceShader = Shader::Create("SimpleIrradianceShader");
		dynamic_cast<SimpleIrradianceShader*>(irrandianceShader)->SetCubeMap(m_CubeMap);
		m_Renderer->BindShader(irrandianceShader);
		m_Renderer->SetViewPort(ts, ts);//这一步很重要
		for (int i = 0; i < 6; i++)
		{
			m_Renderer->Clear();
			irrandianceShader->SetVPMatrix(captureViews[i] * captureProjection);
			dynamic_cast<SimpleIrradianceShader*>(irrandianceShader)->SetIrradianceMap(&irrandianceMaps[i]);
			m_Renderer->DrawMesh(m_SkyBox);

			irrandianceMaps[i].Save(path + "IrradianceMap/" + filesname[i]);
			printf("IrrandianceMap%d计算完成\n", i);
		}
		m_Renderer->SetViewPort(WIDTH, HEIGHT);
		delete irrandianceShader;
	}

	//prefilter env map
	unsigned int maxMipLevels = 6;
	if (GENERATEPREFLITERMAP)
	{
		m_PrefilterCubeMap = new CubeMap(
			Texture(128, 128),
			Texture(128, 128),
			Texture(128, 128),
			Texture(128, 128),
			Texture(128, 128),
			Texture(128, 128)
		);
		m_PrefilterCubeMap->GenerateMipMap(maxMipLevels);
		Shader* prefilterShader = Shader::Create("PrefilterShader");
		dynamic_cast<PrefilterShader*>(prefilterShader)->SetCubeMap(m_CubeMap);
		m_CubeMap->GenerateMipMap(maxMipLevels);

		for (unsigned int mip = 0; mip < maxMipLevels; mip++)
		{
			unsigned int mipWidth = 128 * std::pow(0.5, mip);
			unsigned int mipHeight = 128 * std::pow(0.5, mip);

			std::vector<Texture> prefilterMaps = {
				Texture(mipWidth, mipHeight),
				Texture(mipWidth, mipHeight),
				Texture(mipWidth, mipHeight),
				Texture(mipWidth, mipHeight),
				Texture(mipWidth, mipHeight),
				Texture(mipWidth, mipHeight)
			};

			m_Renderer->BindShader(prefilterShader);
			m_Renderer->SetViewPort(mipWidth, mipHeight);
			float roughness = (float)mip / (float)(maxMipLevels - 1);
			dynamic_cast<PrefilterShader*>(prefilterShader)->SetRoughness(roughness);

			for (int i = 0; i < 6; i++)
			{
				m_Renderer->Clear();
				prefilterShader->SetVPMatrix(captureViews[i] * captureProjection);
				dynamic_cast<PrefilterShader*>(prefilterShader)->SetPrifilterMap(&prefilterMaps[i]);
				m_Renderer->DrawMesh(m_SkyBox);

				prefilterMaps[i].Save(path + std::format("PrefilterMap/mipmap{}_", mip) + filesname[i]);
				printf("PrefilterMap_mip%d_%d计算完成\n", mip, i);
			}

			m_PrefilterCubeMap->SetCubeMap(prefilterMaps, mip);
		}
		m_Renderer->SetViewPort(WIDTH, HEIGHT);
		delete prefilterShader;
	}

	//LUT Map
	if (GENERATELUTMAP)
	{
		Texture BrdfLUTMap(rs, rs);
		m_Renderer->SetViewPort(rs, rs);
		Shader* brdfLUTShader = Shader::Create("BrdfLUTShader");
		Mesh quad("obj/quad.obj");
		dynamic_cast<BrdfLUTShader*>(brdfLUTShader)->SetLUTMap(&BrdfLUTMap);
		m_Renderer->BindShader(brdfLUTShader);
		brdfLUTShader->SetVPMatrix(captureViews[2] * captureProjection);
		m_Renderer->Clear();
		m_Renderer->DrawMesh(&quad);
		BrdfLUTMap.Save(path + "LUTMap/BrdfLUT.tga");
		printf("LUTMap计算完成\n");
		m_Renderer->SetViewPort(WIDTH, HEIGHT);
	}

	//读入irradianceMap
	for (int i = 0; i < 6; i++)
	{
		cubemaps[i] = Texture(path + "IrradianceMap/" + filesname[i]);
	}
	
	m_IrradianceCubeMap = 
		new CubeMap(cubemaps[0], cubemaps[1], cubemaps[2], cubemaps[3], cubemaps[4], cubemaps[5]);

	//读入prefiltermap
	{
		m_PrefilterCubeMap = new CubeMap(
			Texture(128, 128),
			Texture(128, 128),
			Texture(128, 128),
			Texture(128, 128),
			Texture(128, 128),
			Texture(128, 128)
		);
		m_PrefilterCubeMap->GenerateMipMap(maxMipLevels);
		for (unsigned int mip = 0; mip < maxMipLevels; mip++)
		{
			unsigned int mipWidth = 128 * std::pow(0.5, mip);
			unsigned int mipHeight = 128 * std::pow(0.5, mip);

			std::vector<Texture> prefilterMaps = {
				Texture(mipWidth, mipHeight),
				Texture(mipWidth, mipHeight),
				Texture(mipWidth, mipHeight),
				Texture(mipWidth, mipHeight),
				Texture(mipWidth, mipHeight),
				Texture(mipWidth, mipHeight)
			};
			for (int i = 0; i < 6; i++)
			{
				prefilterMaps[i] = Texture(path + std::format("PrefilterMap/mipmap{}_", mip) + filesname[i]);
			}
			m_PrefilterCubeMap->SetCubeMap(prefilterMaps, mip);
		}
	}

	//读入LUT
	m_BrdfLUTMap = new Texture(path + "LUTMap/BrdfLUT.tga");
	
	//dynamic_cast<SkyBoxShader*>(m_SkyBoxShader)->SetCubeMap(m_PrefilterCubeMap);


	//sphere
	m_SphereMesh = new Mesh("obj/sphere.obj");
	m_SphereShader = Shader::Create("IBLShader");
	m_AlbedoMap = new Texture("obj/sphere/rustediron2_basecolor.tga");
	//m_NormalMap = new Texture("obj/sphere/rustediron2_normal.tga");
	m_MetallicMap = new Texture("obj/sphere/rustediron2_metallic.tga");
	m_RoughnessMap = new Texture("obj/sphere/rustediron2_roughness.tga");

	dynamic_cast<IBLShader*>(m_SphereShader)->SetIrradianceMap(m_IrradianceCubeMap);
	dynamic_cast<IBLShader*>(m_SphereShader)->SetPrefilterMap(m_PrefilterCubeMap);
	dynamic_cast<IBLShader*>(m_SphereShader)->SetLUTMap(m_BrdfLUTMap);

}

PIRenderer::Test_IBL::~Test_IBL()
{
	m_CubeMap->DelteAllTextures();
	delete m_CubeMap;
	delete m_IrradianceCubeMap;
	delete m_SkyBoxShader;
	delete m_BrdfLUTMap;
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


	//sphere
	Matrix4 ModelMatrix = Matrix4::Scale(0.01, 0.01, 0.01);
	Matrix4 VPMatrix = m_Controller->GetCamera().GetViewProjectionMatrix();
	m_Renderer->UseBackFaceCulling(true);
	m_Renderer->UseDepthTest(true);

	/*Vector3f lightPos = { (float)(5 * sin(clock() / 10 * PI / 180.0f) * cos(45 * PI / 180.0f)),
							(float)(5 * sin(0 * PI / 180.0f)),
							(float)(5 * cos(clock() / 10 * PI / 180.0f) * cos(45 * PI / 180.0f)) };*/
	Vector3f lightPos = { 0.0f,  0.0f, 10.0f };
	DirectionLight dLight = { -lightPos, lightPos, 0 };
	Matrix4 LightModelMatrix = 
		Matrix4::Scale(0.1, 0.1, 0.1) * Matrix4::Translate(dLight.GetPosition());

	m_SphereShader->SetLight(dLight);
	m_SphereShader->SetModelMatrix(ModelMatrix);
	m_SphereShader->SetVPMatrix(VPMatrix);
	m_SphereShader->SetEyePos(m_Controller->GetCamera().GetPosition());

	/*dynamic_cast<PbrShader*>(m_SphereShader)->SetAlbedoMap(m_AlbedoMap);
	dynamic_cast<PbrShader*>(m_SphereShader)->SetNormalMap(m_NormalMap);
	dynamic_cast<PbrShader*>(m_SphereShader)->SetMetallicMap(m_MetallicMap);
	dynamic_cast<PbrShader*>(m_SphereShader)->SetRoughnessMap(m_RoughnessMap);*/

	dynamic_cast<PbrShader*>(m_SphereShader)->SetAlbedo({ 1.f, 1.f, 1.f });
	dynamic_cast<PbrShader*>(m_SphereShader)->SetAmbientOcclusion(1.0f);
	dynamic_cast<PbrShader*>(m_SphereShader)->SetMetallic(1.0f);
	dynamic_cast<PbrShader*>(m_SphereShader)->SetRoughness(0.0f);

	m_Renderer->BindShader(m_SphereShader);

	//ShowSphereMatrix();

	m_Renderer->DrawMesh(m_SphereMesh);

	m_Window->OnUpdate();
}
