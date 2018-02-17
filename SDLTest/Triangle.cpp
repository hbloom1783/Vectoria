#include "Triangle.h"

#include "SDL2_gfxPrimitives.h"

namespace Geometry
{
	Triangle::Triangle(
		float x0, float y0, float z0,
		float x1, float y1, float z1,
		float x2, float y2, float z2) : p0(x0, y0, z0), p1(x1, y1, z1), p2(x2, y2, z2)
	{
	}

	Triangle::Triangle(Vector3 p0, Vector3 p1, Vector3 p2) : p0(p0), p1(p1), p2(p2)
	{
	}

	Vector3 Triangle::GetP0() const
	{
		return p0;
	}

	Vector3 Triangle::GetP1() const
	{
		return p1;
	}

	Vector3 Triangle::GetP2() const
	{
		return p2;
	}

	Vector2 Triangle::GetNormal(Matrix3 transform) const
	{
		Vector3 u = (transform * this->p1) - (transform * this->p0);
		Vector3 v = (transform * this->p2) - (transform * this->p0);

		return Vector2(
			(u.y * v.z) - (u.z * v.y),
			(u.z * v.x) - (u.x * v.z));
	}

	void Triangle::Render(SDL_Renderer* dst, Matrix3 transform, Vector2 offset, SDL_Color color)
	{
		Vector3 t0 = (transform * p0) + offset.Extend();
		Vector3 t1 = (transform * p1) + offset.Extend();
		Vector3 t2 = (transform * p2) + offset.Extend();

		filledTrigonRGBA(
			dst,
			(int)t0.x, (int)t0.y,
			(int)t1.x, (int)t1.y,
			(int)t2.x, (int)t2.y,
			color.r, color.g, color.b, color.a);
	}

	Triangle Triangle::PerturbVertices(const PerturbPerlin& perturb, float scale)
	{
		return Triangle(
			perturb.PerturbVector(this->p0, scale),
			perturb.PerturbVector(this->p1, scale),
			perturb.PerturbVector(this->p2, scale));
	}
}