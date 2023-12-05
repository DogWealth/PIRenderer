#include "Window.h"

namespace PIRenderer {
	Window::Window(int width, int height, const char* title)
		: m_Width(width), m_Height(height)
	{

		//��ʼ��
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			SDL_Log("Init failed: %s", SDL_GetError());
			SDL_Quit();
		}

		SDL_Log("Init successed!");

		//��������
		m_Window = SDL_CreateWindow("PI Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, 0);

		//��ȡsurface
		m_Surface = SDL_GetWindowSurface(m_Window);

		m_Canvas = new Canvas((uint32_t*)m_Surface->pixels, m_Width, m_Height);
	}

	Window::~Window()
	{
		//����surface
		SDL_FreeSurface(m_Surface);

		//���ٴ���
		SDL_DestroyWindow(m_Window);

		SDL_Quit();

		SDL_Log("Quit!");

		delete m_Canvas;
	}

	void Window::Run()
	{
		while (m_Running)
		{
			//SDL_LockSurface(m_Surface);

			OnEvent();

			Clear();

			//SetPixel();
			m_Canvas->DrawTriangle({ 100, 100, 0 }, { 100, 200, 0 }, { 350, 150, 0 }, { 1, 0, 0 });

			Render();
		}
	}

	void Window::Render()
	{
		//SDL_UnlockSurface(m_Surface);

		//���������ݸ��µ���Ļ��
		SDL_UpdateWindowSurface(m_Window);
	}

	void Window::Clear()
	{
		m_Canvas->Clear();
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
			SDL_Log("SDLK_DOWN!");
			start += 10;
		}
	}
	void Window::SetPixel()
	{
		//�޸��ڴ���ɫ
		for (int i = start; i < start + 200 && i < m_Height; i++)
			for (int j = 0; j < 200; j++)
			{
				m_Canvas->SetPixel(i, j, 0, {1, 1, 0}); //��A��RGB
			}
	}
}
