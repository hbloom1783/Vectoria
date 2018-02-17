#pragma once

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <list>
using std::list;

#include <string>
using std::string;

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
using Color::HSVAColor;

#include "XmlReader.h"
using XmlReader::XmlNode;

#include "Glitching.h"

namespace Models
{
	struct Primitive;

	struct RefData
	{
		map<string, Vector3> points;
		map<string, HSVAColor> colors;
	};

	class Model
	{
	public:
		Model();
		Model(const XmlNode& modelNode);

		void LoadXml(const XmlNode& modelNode, const RefData& refData = RefData());

		void Render(
			SDL_Renderer* renderer,
			Vector2 sunPos,
			Model* rootModel = NULL);

		Vector2 offset;
		float rotation;
		float scale;

		float vertexDistortion;
		DistortionMap* distortionMap;

		float hueDistortion;
		float hueRotation;
		float sLerp;
		float vLerp;

		void AddLineSegment(const string& name, const LineSegment& lineSegment, const HSVAColor& color);
		void AddTriangle(const string& name, const Triangle& triangle, const HSVAColor& color);
		void AddSubmodel(const string& name, const Model& model);

		Primitive& GetPrimitive(const string& name);

		vector<Primitive*> GetAllPrimitives();

		void RemovePrimitive(const string& name);

		Model OffsetModel(const Vector2& offsetBy) const;

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

		//Model* parent;

		PrimType type;
		LineSegment asLine;
		Triangle asTri;
		Model asSubmodel;

		HSVAColor color;
		bool disable;
	};
}