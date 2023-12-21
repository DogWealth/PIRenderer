#include "Window.h"
#include "Mesh.h"
#include "Input.h"

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


	SDL_Surface* Window::GetSurface()
	{
		return m_Surface;
	}

	void Window::OnUpdate()
	{
		//SDL_UnlockSurface(m_Surface);

		//将绘制内容跟新到屏幕上
		SDL_UpdateWindowSurface(m_Window);
	}

}
