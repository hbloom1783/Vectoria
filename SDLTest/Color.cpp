#include "Color.h"

namespace Color
{
	HSVAColor::HSVAColor(float h, float s, float v, float a)
	{
		this->h = h;
		this->s = s;
		this->v = v;
		this->a = a;
	}

	// please don't need this, it sucks to write
	/*HSVAColor::HSVAColor(const SDL_Color& other)
	{
		this->a = other.a;

		Uint8* max;
		Uint8* mid;
		Uint8* min;
	}*/

	SDL_Color HSVAColor::Compile() const
	{
		SDL_Color output = { 0, 0, 0, this->a * 0xFF };

		float modHue = this->h;
		while (modHue < 0)
		{
			modHue += 360;
		}
		while (modHue > 360)
		{
			modHue -= 360;
		}

		int hueSegment = int(modHue / 60.0f);
		float hueLerp = (modHue / 60.0f) - hueSegment;
		if ((hueSegment % 2) == 1)
			hueLerp = 1 - hueLerp;

		Uint8* max;
		Uint8* mid;
		Uint8* min;

		switch (hueSegment % 6)
		{
		case 0:
			max = &(output.r);
			mid = &(output.g);
			min = &(output.b);
			break;

		case 1:
			max = &(output.g);
			mid = &(output.r);
			min = &(output.b);
			break;

		case 2:
			max = &(output.g);
			mid = &(output.b);
			min = &(output.r);
			break;

		case 3:
			max = &(output.b);
			mid = &(output.g);
			min = &(output.r);
			break;

		case 4:
			max = &(output.b);
			mid = &(output.r);
			min = &(output.g);
			break;

		case 5:
			max = &(output.r);
			mid = &(output.b);
			min = &(output.g);
			break;
		}

		float chroma = this->v * this->s;
		float lerp = chroma * hueLerp;
		float floor = this->v - chroma;

		*max = (chroma + floor) * 0xFF;
		*mid = (lerp + floor) * 0xFF;
		*min = (floor) * 0xFF;

		return output;
	}

	HSVAColor HSVAColor::DeltaH(float hDelta) const
	{
		return HSVAColor(
			this->h + hDelta,
			this->s,
			this->v,
			this->a);
	}

	HSVAColor HSVAColor::LerpSVA(float sLerp, float vLerp, float aLerp) const
	{
		return HSVAColor(
			this->h,
			this->s * sLerp,
			this->v * vLerp,
			this->a * aLerp);
	}

	const HSVAColor HSVAColor::Red =
		HSVAColor(000.0, 1.0, 1.0, 1.0);
	const HSVAColor HSVAColor::Yellow =
		HSVAColor(060.0, 1.0, 1.0, 1.0);
	const HSVAColor HSVAColor::Green =
		HSVAColor(120.0, 1.0, 1.0, 1.0);
	const HSVAColor HSVAColor::Cyan =
		HSVAColor(180.0, 1.0, 1.0, 1.0);
	const HSVAColor HSVAColor::Blue =
		HSVAColor(240.0, 1.0, 1.0, 1.0);
	const HSVAColor HSVAColor::Magenta =
		HSVAColor(300.0, 1.0, 1.0, 1.0);
	const HSVAColor HSVAColor::White =
		HSVAColor(000.0, 0.0, 1.0, 1.0);
	const HSVAColor HSVAColor::Black =
		HSVAColor(000.0, 1.0, 0.0, 1.0);

	const HSVAColor HSVAColor::DkRed =
		HSVAColor(000.0, 1.0, 0.5, 1.0);
	const HSVAColor HSVAColor::DkYellow =
		HSVAColor(060.0, 1.0, 0.5, 1.0);
	const HSVAColor HSVAColor::DkGreen =
		HSVAColor(120.0, 1.0, 0.5, 1.0);
	const HSVAColor HSVAColor::DkCyan =
		HSVAColor(180.0, 1.0, 0.5, 1.0);
	const HSVAColor HSVAColor::DkBlue =
		HSVAColor(240.0, 1.0, 0.5, 1.0);
	const HSVAColor HSVAColor::DkMagenta =
		HSVAColor(300.0, 1.0, 0.5, 1.0);
	const HSVAColor HSVAColor::DkWhite =
		HSVAColor(000.0, 0.0, 0.75, 1.0);
	const HSVAColor HSVAColor::DkBlack =
		HSVAColor(000.0, 0.0, 0.25, 1.0);
}