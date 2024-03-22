#include "Application.h"
#include "shader/Blinn_PhongShader.h"
#include "shader/SkyBoxShader.h"

namespace PIRenderer {
	Application::Application()
	{
		m_DepthBuffer = new float[WIDTH * HEIGHT];

		m_Window = new Window(WIDTH, HEIGHT, "PI Renderer");

		uint32_t* frameBuffer = (uint32_t*)(m_Window->GetSurface()->pixels);
		m_Renderer = new PIRenderer::Renderer(frameBuffer, m_DepthBuffer, WIDTH, HEIGHT);
		m_Controller = new PIRenderer::OrbitController(0.1, 50, 90, WIDTH / HEIGHT);

		//m_Controller = new PIRenderer::OrthographicCameraController(-WIDTH / HEIGHT, WIDTH / HEIGHT, -1, 1, 1, - 1);
	}

	Application::~Application()
	{
		delete m_Window;
		delete m_Renderer;
		delete m_Controller;
		delete[] m_DepthBuffer;
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