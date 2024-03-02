#include "Application.h"

#define WIDTH	800.0f
#define HEIGHT	600.0f

namespace PIRenderer {
	Application::Application()
	{
		m_Window = new Window(WIDTH, HEIGHT, "PI Renderer");
		m_Renderer = new PIRenderer::Renderer((uint32_t*)(m_Window->GetSurface()->pixels), WIDTH, HEIGHT);
		m_Mesh = new PIRenderer::Mesh("obj/floor.obj");//注意相对路径时相对PIRenderer这个文件夹开始的
		m_Shader = PIRenderer::Shader::Create("BasicShader");
		m_Texture = new PIRenderer::Texture("obj/floor_diffuse.tga");
		m_Controller = new PIRenderer::OrbitController(1, 50, 90, WIDTH / HEIGHT);

		m_HeadMesh = new PIRenderer::Mesh("obj/african_head.obj");
		m_HeadShader = PIRenderer::Shader::Create("BasicShader");
		m_HeadTexture = new PIRenderer::Texture("obj/african_head_diffuse.tga");

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
	}

	void Application::Run()
	{
		double old_time = clock();
		while (m_Running)
		{
			double time = clock();
			printf("FPS: %lf\n", 1000 / (time - old_time));
			old_time = time;
			//SDL_LockSurface(m_Surface);

			OnEvent();
			OnUpdate();
		}
	}

	void Application::OnUpdate()
	{
		m_Renderer->Clear();

		m_Controller->OnUpdate();
		Matrix4 VPMatrix = m_Controller->GetCamera().GetViewProjectionMatrix();
		Matrix4 ModelMatrix = Matrix4::Scale(0.1, 0.1, 0.1) * Matrix4::RotateX(90) * Matrix4::Translate(0, -1, 0);

		m_Renderer->BindShader(m_Shader);
		m_Shader->SetVPMatrix(VPMatrix);
		m_Shader->SetModelMatrix(ModelMatrix);

		Render(m_Mesh);

		m_Renderer->BindShader(m_HeadShader);
		m_HeadShader->SetVPMatrix(VPMatrix);

		Render(m_HeadMesh);

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