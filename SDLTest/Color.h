#pragma once

#include "SDL2_gfxPrimitives.h"

namespace Color
{
	class HSVAColor
	{
	public:
		HSVAColor(float h = 0.0f, float s = 1.0f, float v = 1.0f, float a = 1.0f);
		HSVAColor(const SDL_Color& other);

		SDL_Color Compile() const;

		HSVAColor Lerp(float sLerp, float vLerp, float aLerp = 1.0f) const;

		const static HSVAColor Red;
		const static HSVAColor Yellow;
		const static HSVAColor Green;
		const static HSVAColor Cyan;
		const static HSVAColor Blue;
		const static HSVAColor Magenta;
		const static HSVAColor White;
		const static HSVAColor Black;

		const static HSVAColor dkRed;
		const static HSVAColor dkYellow;
		const static HSVAColor dkGreen;
		const static HSVAColor dkCyan;
		const static HSVAColor dkBlue;
		const static HSVAColor dkMagenta;
		const static HSVAColor dkWhite;
		const static HSVAColor dkBlack;
	private:
		float h, s, v, a;
	};
}
