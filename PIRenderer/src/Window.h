#pragma once
#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <SDL.h>
#include "Renderer.h"
#include "CameraController.h"

namespace PIRenderer {
	class Window
	{
	public:
		Window(int width, int height, const char* title);
		~Window();

		void OnUpdate();

		SDL_Surface* GetSurface();

	private:
		int m_Width;
		int m_Height;

		SDL_Surface* m_Surface;
		SDL_Window* m_Window;

	};
}