#pragma once

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <list>
using std::list;

#include "SDL.h"

#include "Matrix.h"
using Geometry::Vector2;
using Geometry::Vector3;
using Geometry::Matrix3;

#include "Renderable.h"
using Geometry::Renderable;

#include "Triangle.h"
using Geometry::Triangle;

#include "LineSegment.h"
using Geometry::LineSegment;

#include "Color.h"

namespace Models
{
	struct Primitive;

	class Model
	{
	public:
		Model();

		void Render(SDL_Renderer* renderer, Vector2 sunPos, Matrix3 parentTransform = Matrix3::Identity, Vector2 parentOffset = Vector2::Origin);

		Model* parent;
		Model& GetRootModel();

		Vector2 offset;
		float rotation;
		float scale;

		float huePerturbation;
		float vertexPerturbation;

		void AddLineSegment(const string& name, const LineSegment& lineSegment, const HSVAColor& color);
		void AddTriangle(const string& name, const Triangle& triangle, const HSVAColor& color);
		void AddSubmodel(const string& name, const Model& model);

		Primitive& GetPrimitive(const string& name);

		void RemovePrimitive(const string& name);

	private:
		map<string, Primitive> prims;
		list<string> drawOrder;
	};

	enum PrimType
	{
		primInvalid,
		primLineSegment,
		primTriangle,
		primSubmodel,
		primTypeCount,
	};

	struct Primitive
	{
		Primitive(Model* parent = NULL);
		Primitive(Model* parent, const LineSegment& line, const HSVAColor& color);
		Primitive(Model* parent, const Triangle& triangle, const HSVAColor& color);
		Primitive(Model* parent, const Model& submodel);

		Primitive(const Primitive& other);
		Primitive& operator=(const Primitive& other);

		Model* parent;

		PrimType type;
		LineSegment asLine;
		Triangle asTri;
		Model asSubmodel;

		HSVAColor color;
	};
}