#include "Window.h"
#include "Mesh.h"
#include "Input.h"

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
		m_Window = SDL_CreateWindow("PI Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

		//��ȡsurface
		m_Surface = SDL_GetWindowSurface(m_Window);

		
	}

	Window::~Window()
	{
		//����surface
		SDL_FreeSurface(m_Surface);

		//���ٴ���
		SDL_DestroyWindow(m_Window);

		SDL_Quit();

		SDL_Log("Quit!");
	}


	SDL_Surface* Window::GetSurface()
	{
		return m_Surface;
	}

	void Window::OnUpdate()
	{
		//SDL_UnlockSurface(m_Surface);

		//���������ݸ��µ���Ļ��
		SDL_UpdateWindowSurface(m_Window);
	}

}
