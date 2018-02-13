#include "Model.h"

#include "SDL.h"

#include "Matrix.h"
using Geometry::Matrix2;
using Geometry::Matrix3;
using Geometry::RotationMatrix;
using Geometry::ScaleMatrix;
using Geometry::CalculateAngle;

#include "Color.h"

namespace Models
{
	Primitive::Primitive()
	{
		this->type = primInvalid;
	}

	Primitive::Primitive(const LineSegment& line, SDL_Color color)
	{
		this->type = primLineSegment;
		this->asLine = line;
		this->color = color;
	}

	Primitive::Primitive(const Triangle& triangle, SDL_Color color)
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
		this->color =other.color;
	}

	Model::Model()
	{
		this->rotation = 0;
		this->scale = 1;
	}

	void Model::Render(SDL_Renderer* renderer, Vector2 sunPos)
	{
		Matrix3 transform = RotationMatrix(this->rotation).Extend() * ScaleMatrix(this->scale);
		for (auto iter = this->prims.begin(); iter != this->prims.end(); iter++)
		{
			if (iter->type == primLineSegment)
			{
				iter->asLine.Render(
					renderer,
					transform,
					this->offset,
					iter->color);
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

				//litAngle = sqrt(litAngle);
				//litAngle *= litAngle;
				litAngle = (sqrt(litAngle) + (litAngle*litAngle)) / 2;
				//litAngle = sqrt(sqrt(litAngle));

				SDL_Color litColor = LerpV(
					iter->color,
					litAngle);

				iter->asTri.Render(
					renderer,
					transform,
					this->offset,
					litColor);
			}
		}
	}
}