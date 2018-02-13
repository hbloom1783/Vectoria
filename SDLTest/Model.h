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

#include "Color.h"

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
		Primitive(const LineSegment& line, HSVAColor color);
		Primitive(const Triangle& triangle, HSVAColor color);

		Primitive(const Primitive& other);

		PrimType type;
		LineSegment asLine;
		Triangle asTri;

		HSVAColor color;
	};

	struct Model
	{
		Model();

		void Render(SDL_Renderer* renderer, Vector2 sunPos);

		Vector2 offset;
		float rotation;
		float scale;

		float huePerturbation;
		float vertexPerturbation;

		vector<Primitive> prims;
	};
}