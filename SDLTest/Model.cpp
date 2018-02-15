#include "Model.h"

#include <assert.h>

#include "SDL.h"

#include "Matrix.h"
using Geometry::Matrix2;
using Geometry::Matrix3;
using Geometry::CalculateAngle;

#include "Color.h"

namespace Models
{
	#pragma region Model

	Model::Model()
	{
		this->parent = NULL;

		this->rotation = 0;
		this->scale = 1;
		this->huePerturbation = 0;
		this->vertexPerturbation = 0;
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

	void Model::Render(SDL_Renderer* renderer, Vector2 sunPos, Matrix3 parentTransform, Vector2 parentOffset)
	{
		Matrix3 transform = parentTransform * Matrix2::RotationMatrix(this->rotation).Extend() * Matrix3::ScaleMatrix(this->scale);
		Vector2 offset = parentOffset + this->offset;
		float hueDelta = PerturbStandard(this->huePerturbation);

		for (auto iter = this->drawOrder.begin(); iter != this->drawOrder.end(); iter++)
		{
			string& name = *iter;
			Primitive& prim = this->prims[name];

			if (prim.type == primLineSegment)
			{
				prim.asLine.PerturbVertices(this->vertexPerturbation / this->scale).Render(
					renderer,
					transform,
					offset,
					prim.color.ShiftHue(hueDelta).Compile());
			}
			else if (prim.type == primTriangle)
			{
				float litAngle = CalculateAngle(
					prim.asTri.GetNormal(transform),
					sunPos - offset);

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

				prim.asTri.Render(
					renderer,
					transform,
					offset,
					prim.color.Lerp(1.0, litAngle).ShiftHue(hueDelta).Compile());
			}
			else if (prim.type == primSubmodel)
			{
				prim.asSubmodel.Render(
					renderer,
					sunPos,
					transform,
					offset);
			}
		}*/
	}

	Model& Model::GetRootModel()
	{
		if (this->parent == NULL)
		{
			return *this;
		}
		else
		{
			return this->parent->GetRootModel();
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
		this->asSubmodel.parent = parent;
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