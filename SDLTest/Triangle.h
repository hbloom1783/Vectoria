#pragma once

#include "Matrix.h"
#include "Renderable.h"

#include "Perturbation.h"
using Perturb::PerturbPerlin;

namespace Geometry
{
	class Triangle : public Renderable
	{
	public:
		Triangle(
			float x0 = 0, float y0 = 0, float z0 = 0,
			float x1 = 0, float y1 = 0, float z1 = 0,
			float x2 = 0, float y2 = 0, float z2 = 0);

		Triangle(Vector3 p0, Vector3 p1, Vector3 p2);

		Vector3 GetP0() const;
		Vector3 GetP1() const;
		Vector3 GetP2() const;

		Vector2 GetNormal(Matrix3 transform) const;

		virtual void Render(
			SDL_Renderer* dst,
			Matrix3 transform,
			Vector2 offset,
			SDL_Color color);

		Triangle PerturbVertices(const PerturbPerlin& perturb, float scale = 1.0f);

	private:
		Vector3 p0;
		Vector3 p1;
		Vector3 p2;
	};

}