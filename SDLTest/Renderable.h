#pragma once

#include "SDL.h"

#include "Matrix.h"

namespace Geometry
{
	class Renderable
	{
	public:
		virtual void Render(SDL_Renderer* dst, Matrix3 transform, Vector2 offset, SDL_Color color) = 0;
	};
}