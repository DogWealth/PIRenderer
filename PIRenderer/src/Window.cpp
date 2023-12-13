#include "Window.h"

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
		m_Window = SDL_CreateWindow("PI Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, 0);

		//获取surface
		m_Surface = SDL_GetWindowSurface(m_Window);

		m_Canvas = new Renderer((uint32_t*)m_Surface->pixels, m_Width, m_Height);
	}

	Window::~Window()
	{
		//销毁surface
		SDL_FreeSurface(m_Surface);

		//销毁窗口
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

			Vector3 p1(100, 100, 0), p2(200, 100, 0), p3(350, 150, 0);
			Vertex v1(p1, p1, 0, 0, {1, 0, 0});
			Vertex v2(p2, p1, 0, 0, { 0, 1, 0 });
			Vertex v3(p3, p1, 0, 0, { 0, 0, 1 });

			m_Canvas->DrawTriangle(v1, v2, v3);

			//SetPixel();
			m_Canvas->SetRotationMatrix(0, 0, m_Rotation);
			//m_Canvas->DrawTriangle({ 100, 100, 0 }, { 100, 200, 0 }, { 350, 150, 0 }, { 1, 0, 0 });
			//m_Canvas->DrawTriangle({ 77.024323, 118.605453, 0 }, { 56.23377, 216.420349, 0 }, { 311.166260, 219.489319, 0 }, { 1, 0, 0 });

			Render();
		}
	}

	void Window::Render()
	{
		//SDL_UnlockSurface(m_Surface);

		//将绘制内容跟新到屏幕上
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
	void Window::SetPixel()
	{
		//修改内存颜色
		for (int i = start; i < start + 200 && i < m_Height; i++)
			for (int j = 0; j < 200; j++)
			{
				m_Canvas->SetPixel(i, j, {1, 1, 0}); //（A）RGB
			}
	}
}
