#pragma once

#include "SDL.h"

#include "Matrix.h"
#include "Renderable.h"

namespace Geometry
{
	class LineSegment : public Renderable
	{
	public:
		LineSegment(Vector3 p0 = Vector3::Origin, Vector3 p1 = Vector3::Origin);

		Vector3 GetP0() const;
		Vector3 GetP1() const;

		virtual void Render(
			SDL_Renderer* dst,
			Matrix3 transform,
			Vector2 offset,
			SDL_Color color);

	private:
		Vector3 p0;
		Vector3 p1;
	};
}
