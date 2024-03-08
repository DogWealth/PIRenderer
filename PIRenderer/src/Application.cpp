#include "Application.h"
#include "shader/Blinn_PhongShader.h"
#define WIDTH	800.0f
#define HEIGHT	600.0f

namespace PIRenderer {
	Application::Application()
	{
		m_DepthBuffer = new float[WIDTH * HEIGHT];
		m_ShadowMap = new float[WIDTH * HEIGHT];
		m_Window = new Window(WIDTH, HEIGHT, "PI Renderer");

		uint32_t* frameBuffer = (uint32_t*)(m_Window->GetSurface()->pixels);
		m_Renderer = new PIRenderer::Renderer(frameBuffer, m_DepthBuffer, WIDTH, HEIGHT);
		m_Mesh = new PIRenderer::Mesh("obj/floor.obj");//注意相对路径时相对PIRenderer这个文件夹开始的
		m_Shader = PIRenderer::Shader::Create("Blinn_PhongShader");
		m_Texture = new PIRenderer::Texture("obj/floor_diffuse.tga");
		m_Controller = new PIRenderer::OrbitController(1, 50, 90, WIDTH / HEIGHT);

		m_HeadMesh = new PIRenderer::Mesh("obj/african_head.obj");
		m_HeadShader = PIRenderer::Shader::Create("Blinn_PhongShader");
		m_HeadTexture = new PIRenderer::Texture("obj/african_head_diffuse.tga");

		m_LightMesh = new PIRenderer::Mesh("obj/cube.obj");
		m_LightShader = PIRenderer::Shader::Create("LightShader");

		m_SimpleDepthShader = PIRenderer::Shader::Create("SimpleDepthShader");

		//m_Controller = new PIRenderer::OrthographicCameraController(-WIDTH / HEIGHT, WIDTH / HEIGHT, -1, 1, 1, - 1);

		//m_Renderer->AddMesh(m_Mesh);
		
		//m_Shader->SetTexture(m_Texture);
		m_HeadShader->SetTexture(m_HeadTexture);
	}

	Application::~Application()
	{
		delete m_Window;
		delete m_Renderer;
		delete m_Mesh;
		delete m_Shader;
		delete m_Texture;
		delete m_Controller;
		delete m_HeadMesh;
		delete m_HeadShader;
		delete m_HeadTexture;
		delete m_LightMesh;
		delete m_LightShader;
		delete m_DepthBuffer;
		delete m_ShadowMap;
		delete m_SimpleDepthShader;
	}

	void Application::Run()
	{
		double old_time = clock();
		while (m_Running)
		{
			double time = clock();
			double tick = time - old_time;
			printf("FPS: %lf\n", 1000 / tick);
			old_time = time;
			//SDL_LockSurface(m_Surface);

			OnEvent();
			OnUpdate(tick);
		}
	}

	void Application::OnUpdate(double tick)
	{
		m_Renderer->Clear();

		m_Controller->OnUpdate(tick);
		Matrix4 VPMatrix = m_Controller->GetCamera().GetViewProjectionMatrix();
		Matrix4 ModelMatrix = Matrix4::Scale(0.1, 0.1, 0.1) * Matrix4::RotateX(90) * Matrix4::Translate(0, -1, 0);
		Matrix4 HeadModelMatrix = Matrix4::Scale(1, 1, 1);

		Vector3f lightPos = { (float)(2 * sin(clock() / 10 * PI / 180.0f) * cos(60 * PI / 180.0f)),
								(float)(2 * sin(60 * PI / 180.0f)),
								(float)(2 * cos(clock() / 10 * PI / 180.0f) * cos(60 * PI / 180.0f)) };

		DirectionLight dLight = { -lightPos, lightPos, 1};

		Matrix4 LightModelMatrix = Matrix4::Scale(0.1, 0.1, 0.1) *  Matrix4::Translate(dLight.GetPosition());

		Matrix4 LightSpaceMatrix =	Matrix4::LookAt(lightPos, lightPos, { 0, 1, 0 }) * 
									Matrix4::Orthographic(-5, 5, -5, 5, 50, 1);

		//caculate shadowmap
		m_Renderer->SetDepthBuffer(m_ShadowMap);
		m_Renderer->Clear();

		m_Renderer->BindShader(m_SimpleDepthShader);
		m_SimpleDepthShader->SetVPMatrix(LightSpaceMatrix);
		m_SimpleDepthShader->SetModelMatrix(HeadModelMatrix);
		Render(m_HeadMesh);

		m_SimpleDepthShader->SetModelMatrix(ModelMatrix);
		Render(m_Mesh);

		dynamic_cast<Blinn_PhongShader*>(m_Shader)->SetShaowMap(m_ShadowMap, WIDTH, HEIGHT);
		dynamic_cast<Blinn_PhongShader*>(m_Shader)->UseShadow(true);
		dynamic_cast<Blinn_PhongShader*>(m_Shader)->SetLightSpaceMatrix(LightSpaceMatrix);


		//common render
		m_Renderer->SetDepthBuffer(m_DepthBuffer);
		m_Renderer->Clear();

		m_Shader->SetLight(dLight);
		m_HeadShader->SetLight(dLight);

		m_Renderer->BindShader(m_Shader);
		m_Shader->SetVPMatrix(VPMatrix);
		m_Shader->SetModelMatrix(ModelMatrix);

		Render(m_Mesh);

		m_Renderer->BindShader(m_HeadShader);
		m_HeadShader->SetVPMatrix(VPMatrix);
		m_HeadShader->SetModelMatrix(HeadModelMatrix);

		Render(m_HeadMesh);

		m_Renderer->BindShader(m_LightShader);
		m_LightShader->SetVPMatrix(VPMatrix);
		m_LightShader->SetModelMatrix(LightModelMatrix);
		m_LightShader->SetEyePos(m_Controller->GetCamera().GetPosition());

		Render(m_LightMesh);

		m_Window->OnUpdate();
	}

	void Application::OnEvent()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				m_Running = false;
				break;
			}
			/*else if (event.type = SDL_KEYDOWN)
			{
				OnKeyEvent(event);
			}*/

		}
	}

	void Application::Render()
	{
		m_Renderer->Clear();
		m_Renderer->DrawMeshs();
	}

	void Application::Render(Mesh* mesh)
	{
		m_Renderer->DrawMesh(mesh);
	}

}