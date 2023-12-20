#include "Window.h"
#include "Mesh.h"
#include <time.h>

namespace PIRenderer {
	Window::Window(int width, int height, const char* title)
		: m_Width(width), m_Height(height)
	{

		//初始化
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			SDL_Log("Init failed: %s", SDL_GetError());
			SDL_Quit();
		}

		SDL_Log("Init successed!");

		//创建窗口
		m_Window = SDL_CreateWindow("PI Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

		//获取surface
		m_Surface = SDL_GetWindowSurface(m_Window);

		m_Renderer = nullptr;

		//test
		
	}

	Window::~Window()
	{
		//销毁surface
		SDL_FreeSurface(m_Surface);

		//销毁窗口
		SDL_DestroyWindow(m_Window);

		SDL_Quit();

		SDL_Log("Quit!");
	}

	void Window::Run()
	{
		double old_time = clock();
		while (m_Running)
		{
			double time = clock();
			printf("FPS: %lf\n", 1000 / (time - old_time));
			old_time = time;
			//SDL_LockSurface(m_Surface);

			OnEvent();

			Clear();

			Render();
		}
	}

	void Window::SetRenderer(Renderer* renderer)
	{
		m_Renderer = renderer;
	}

	SDL_Surface* Window::GetSurface()
	{
		return m_Surface;
	}

	void Window::Render()
	{
		//SDL_UnlockSurface(m_Surface);

		m_Renderer->DrawMeshs();
		//m_Renderer->DrawMeshLines();

		m_Renderer->SetRotationMatrix(0, 0, m_Rotation);

		//将绘制内容跟新到屏幕上
		SDL_UpdateWindowSurface(m_Window);
	}

	void Window::Clear()
	{
		m_Renderer->Clear();
	}

	void Window::OnEvent()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				m_Running = false;
				break;
			}
			else if (event.type = SDL_KEYDOWN)
			{
				OnKeyEvent(event);
			}

		}
	}

	void Window::OnKeyEvent(const SDL_Event& event)
	{
		SDL_Keycode KeyCode = event.key.keysym.sym;
		if (KeyCode == SDLK_DOWN)
		{
			//SDL_Log("SDLK_DOWN!");
			start += 10;
			m_Rotation += 2;
		}

		if (KeyCode == SDLK_UP)
		{
			//SDL_Log("SDLK_DOWN!");
			start += 10;
			m_Rotation -= 2;
		}
	}
}
