#pragma once

#include <vector>
using std::vector;

#include "SDL.h"

#include "Matrix.h"
using Geometry::Vector2;

#include "Renderable.h"
using Geometry::Renderable;

#include "Triangle.h"
using Geometry::Triangle;

#include "LineSegment.h"
using Geometry::LineSegment;

namespace Models
{
	enum PrimType
	{
		primInvalid,
		primLineSegment,
		primTriangle,
		primTypeCount,
	};

	struct Primitive
	{
		Primitive();
		Primitive(const LineSegment& line, SDL_Color color);
		Primitive(const Triangle& triangle, SDL_Color color);

		Primitive(const Primitive& other);

		PrimType type;
		LineSegment asLine;
		Triangle asTri;

		SDL_Color color;
	};

	struct Model
	{
	public:
		Model();

		void Render(SDL_Renderer* renderer, Vector2 sunPos);

		Vector2 offset;
		float rotation;
		float scale;

		vector<Primitive> prims;
	};
}