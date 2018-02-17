#include "Model.h"

#include <assert.h>

#include "SDL.h"

#include "Matrix.h"
using Geometry::Matrix2;
using Geometry::Matrix3;
using Geometry::CalculateAngle;

#include "Color.h"

#include "Glitching.h"

namespace Models
{
	#pragma region Model

	Model::Model()
	{
		this->offset = Vector2::Origin;
		this->rotation = 0;
		this->scale = 1;

		this->vertexDistortion = 0.0f;
		this->distortionMap = NULL;

		this->hueDistortion = 0.0f;
	}

	void Model::AddLineSegment(const string& name, const LineSegment& lineSegment, const HSVAColor& color)
	{
		assert(this->prims.find(name) == this->prims.end());

		this->prims[name] = Primitive(this, lineSegment, color);
		this->drawOrder.push_back(name);
	}

	void Model::AddTriangle(const string& name, const Triangle& triangle, const HSVAColor& color)
	{
		assert(this->prims.find(name) == this->prims.end());

		this->prims[name] = Primitive(this, triangle, color);
		this->drawOrder.push_back(name);
	}

	void Model::AddSubmodel(const string& name, const Model& model)
	{
		assert(this->prims.find(name) == this->prims.end());

		this->prims[name] = Primitive(this, model);
		this->drawOrder.push_back(name);
	}

	Primitive& Model::GetPrimitive(const string& name)
	{
		assert(this->prims.find(name) != this->prims.end());

		return this->prims[name];
	}

	void Model::RemovePrimitive(const string& name)
	{
		assert(this->prims.find(name) != this->prims.end());

		this->prims.erase(name);
		this->drawOrder.remove(name);
	}
	
	void Model::Render(
		SDL_Renderer* renderer,
		Vector2 sunPos,
		Model* rootModel)
	{
		Model* realRootModel = rootModel;
		if (rootModel == NULL)
		{
			realRootModel = this;
			this->distortionMap = new DistortionMap(this->vertexDistortion);
		}

		Matrix3 transform =
			Matrix2::RotationMatrix(realRootModel->rotation).Extend() *
			Matrix3::ScaleMatrix(realRootModel->scale);
		Vector2 rootOffset = realRootModel->offset;
		float hueDelta = RangeNormal(realRootModel->hueDistortion);

		for (auto iter = this->drawOrder.begin(); iter != this->drawOrder.end(); iter++)
		{
			string& name = *iter;
			Primitive& prim = this->prims[name];

			if (prim.type == primLineSegment)
			{
				LineSegment distorted = realRootModel->distortionMap->Distort(prim.asLine);
				distorted.Render(
					renderer,
					transform,
					rootOffset,
					prim.color.DeltaH(hueDelta).Compile());
			}
			else if (prim.type == primTriangle)
			{
				Triangle distorted = realRootModel->distortionMap->Distort(prim.asTri);

				float litAngle = CalculateAngle(
					distorted.GetNormal(transform),
					sunPos - rootOffset);

				litAngle /= 180.0;
				litAngle = 1.0f - litAngle;

				if (litAngle > 1)
					litAngle = 1;
				else if (litAngle < 0)
					litAngle = 0;

				// Leaving the other lighting models around to play with
				//litAngle = sqrt(litAngle);
				//litAngle *= litAngle;
				litAngle = (sqrt(litAngle) + (litAngle*litAngle)) / 2;
				//litAngle = sqrt(sqrt(litAngle));

				distorted.Render(
					renderer,
					transform,
					rootOffset,
					prim.color.DeltaH(hueDelta).LerpSVA(1.0, litAngle).Compile());
			}
			else if (prim.type == primSubmodel)
			{
				prim.asSubmodel.Render(
					renderer,
					sunPos,
					realRootModel);
			}
		}

		if (rootModel == NULL)
		{
			delete this->distortionMap;
		}
	}

	void Model::LoadXml(const XmlNode& modelNode, const RefData& refData)
	{
		RefData myRefData = refData;

		vector<XmlNode> children = modelNode.GetChildren();
		for (auto iter = children.begin(); iter != children.end(); iter++)
		{
			if (iter->GetName() == "point")
			{
				myRefData.points[iter->GetAttribute("id")] = Vector3(
					iter->GetAttribute("x").AsFloat(),
					iter->GetAttribute("y").AsFloat(),
					iter->GetAttribute("z", "0").AsFloat());
			}
			else if (iter->GetName() == "color")
			{
				myRefData.colors[iter->GetAttribute("id")] = HSVAColor(
					iter->GetAttribute("h").AsFloat(),
					iter->GetAttribute("s", "1").AsFloat(),
					iter->GetAttribute("v", "1").AsFloat(),
					iter->GetAttribute("a", "1").AsFloat());
			}
			else if (iter->GetName() == "linesegment")
			{
				// Deref points
				Vector3 p0 = myRefData.points.at(iter->GetAttribute("p0"));
				Vector3 p1 = myRefData.points.at(iter->GetAttribute("p1"));

				// Deref color
				HSVAColor color = myRefData.colors.at(iter->GetAttribute("color"));

				this->AddLineSegment(
					iter->GetAttribute("id"),
					LineSegment(p0, p1),
					color);
			}
			else if (iter->GetName() == "triangle")
			{
				// Deref points
				Vector3 p0 = myRefData.points.at(iter->GetAttribute("p0"));
				Vector3 p1 = myRefData.points.at(iter->GetAttribute("p1"));
				Vector3 p2 = myRefData.points.at(iter->GetAttribute("p2"));

				// Deref color
				HSVAColor color = myRefData.colors.at(iter->GetAttribute("color"));

				this->AddTriangle(
					iter->GetAttribute("id"),
					Triangle(p0, p1, p2),
					color);
			}
			else if (iter->GetName() == "model")
			{
				Model subModel;

				subModel.LoadXml(*iter, myRefData);

				this->AddSubmodel(iter->GetAttribute("id"), subModel);
			}
		}
	}

	#pragma endregion

	#pragma region Primitive

	Primitive::Primitive(Model* parent) : parent(parent)
	{
		this->type = primInvalid;
	}

	Primitive::Primitive(Model* parent, const LineSegment& line, const HSVAColor& color) : parent(parent)
	{
		this->type = primLineSegment;
		this->asLine = line;
		this->color = color;
	}

	Primitive::Primitive(Model* parent, const Triangle& triangle, const HSVAColor& color) : parent(parent)
	{
		this->type = primTriangle;
		this->asTri = triangle;
		this->color = color;
	}

	Primitive::Primitive(Model* parent, const Model& submodel) : parent(parent)
	{
		this->type = primSubmodel;
		this->asSubmodel = submodel;
	}

	Primitive::Primitive(const Primitive& other) : parent(other.parent)
	{
		if (other.type == primTriangle)
		{
			this->type = primTriangle;
			this->asTri = other.asTri;
		}
		else if (other.type == primLineSegment)
		{
			this->type = primLineSegment;
			this->asLine = other.asLine;
		}
		else if (other.type == primSubmodel)
		{
			this->type = primSubmodel;
			this->asSubmodel = other.asSubmodel;
		}
		this->color = other.color;
	}

	Primitive& Primitive::operator=(const Primitive& other)
	{
		this->parent = other.parent;
		if (other.type == primTriangle)
		{
			this->type = primTriangle;
			this->asTri = other.asTri;
		}
		else if (other.type == primLineSegment)
		{
			this->type = primLineSegment;
			this->asLine = other.asLine;
		}
		else if (other.type == primSubmodel)
		{
			this->type = primSubmodel;
			this->asSubmodel = other.asSubmodel;
		}
		this->color = other.color;

		return *this;
	}

	#pragma endregion
}