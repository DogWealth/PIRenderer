#pragma once
#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <SDL.h>
#include "Renderer.h"

namespace PIRenderer {
	class Window
	{
	public:
		Window(int width, int height, const char* title);
		~Window();

		void Run();

		void Render();
		void Clear();

		void OnEvent();
		void OnKeyEvent(const SDL_Event& event);

		void SetPixel();

	private:
		int m_Width;
		int m_Height;

		bool m_Running = true;

		SDL_Surface* m_Surface;
		SDL_Window* m_Window;

		Renderer* m_Canvas;

		int start = 0;

		float m_Rotation = -60;

	};
}