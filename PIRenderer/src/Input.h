#pragma once
#include <SDL.h>
namespace PIRenderer {
	class Input
	{
	public:
		static bool IsKeyPressed(SDL_Scancode scancode);
	};

}
