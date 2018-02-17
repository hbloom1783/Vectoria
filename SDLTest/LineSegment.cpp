#include "LineSegment.h"

#include "SDL2_gfxPrimitives.h"

namespace Geometry
{
	LineSegment::LineSegment(Vector3 p0, Vector3 p1) : p0(p0), p1(p1)
	{
	}

	Vector3 LineSegment::GetP0() const
	{
		return p0;
	}

	Vector3 LineSegment::GetP1() const
	{
		return p1;
	}

	void LineSegment::Render(SDL_Renderer* dst, Matrix3 transform, Vector2 offset, SDL_Color color)
	{
		Vector3 t0 = (transform * p0) + offset.Extend();
		Vector3 t1 = (transform * p1) + offset.Extend();

		lineRGBA(
			dst,
			(int)t0.x, (int)t0.y,
			(int)t1.x, (int)t1.y,
			color.r, color.g, color.b, color.a);
	}
}