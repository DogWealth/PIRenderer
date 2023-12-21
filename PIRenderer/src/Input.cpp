#include "Input.h"

namespace PIRenderer {
	bool Input::IsKeyPressed(SDL_Scancode scancode)
	{
		const Uint8* state = SDL_GetKeyboardState(nullptr);

		if (state[scancode])
			return true;

		return false;
	}
}