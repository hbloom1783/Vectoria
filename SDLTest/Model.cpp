#include "Model.h"

#include <random>

#include "SDL.h"

#include "Matrix.h"
using Geometry::Matrix2;
using Geometry::Matrix3;
using Geometry::CalculateAngle;

#include "Color.h"

namespace Models
{
	#pragma region Primitive

	Primitive::Primitive()
	{
		this->type = primInvalid;
	}

	Primitive::Primitive(const LineSegment& line, HSVAColor color)
	{
		this->type = primLineSegment;
		this->asLine = line;
		this->color = color;
	}

	Primitive::Primitive(const Triangle& triangle, HSVAColor color)
	{
		this->type = primTriangle;
		this->asTri = triangle;
		this->color = color;
	}

	Primitive::Primitive(const Primitive& other)
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

	#pragma endregion

	#pragma region Model

	Model::Model()
	{
		this->rotation = 0;
		this->scale = 1;
		this->huePerturbation = 0;
		this->vertexPerturbation = 0;
	}

	void Model::Render(SDL_Renderer* renderer, Vector2 sunPos)
	{
		Matrix3 transform = Matrix2::RotationMatrix(this->rotation).Extend() * Matrix3::ScaleMatrix(this->scale);
		for (auto iter = this->prims.begin(); iter != this->prims.end(); iter++)
		{
			if (iter->type == primLineSegment)
			{
				iter->asLine.PerturbVertices(this->vertexPerturbation / this->scale).Render(
					renderer,
					transform,
					this->offset,
					iter->color.PerturbHue(this->huePerturbation).Compile());
			}
			else if (iter->type == primTriangle)
			{
				float litAngle = CalculateAngle(
					iter->asTri.GetNormal(transform),
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

				iter->asTri.PerturbVertices(this->vertexPerturbation / this->scale).Render(
					renderer,
					transform,
					this->offset,
					iter->color.Lerp(1.0, litAngle).PerturbHue(this->huePerturbation).Compile());
			}
		}
	}

	#pragma endregion
}