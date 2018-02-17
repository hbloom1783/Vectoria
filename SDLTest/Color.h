#pragma once

#include "SDL2_gfxPrimitives.h"

namespace Color
{
	class HSVAColor
	{
	public:
		HSVAColor(float h = 0.0f, float s = 1.0f, float v = 1.0f, float a = 1.0f);
		//HSVAColor(const SDL_Color& other);

		SDL_Color Compile() const;

		HSVAColor DeltaH(float hDelta) const;
		HSVAColor LerpSVA(float sLerp, float vLerp, float aLerp = 1.0f) const;

		const static HSVAColor Red;
		const static HSVAColor Yellow;
		const static HSVAColor Green;
		const static HSVAColor Cyan;
		const static HSVAColor Blue;
		const static HSVAColor Magenta;
		const static HSVAColor White;
		const static HSVAColor Black;

		const static HSVAColor DkRed;
		const static HSVAColor DkYellow;
		const static HSVAColor DkGreen;
		const static HSVAColor DkCyan;
		const static HSVAColor DkBlue;
		const static HSVAColor DkMagenta;
		const static HSVAColor DkWhite;
		const static HSVAColor DkBlack;
	private:
		float h, s, v, a;
	};
}
