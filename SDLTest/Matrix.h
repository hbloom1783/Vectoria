#pragma once

namespace Geometry
{
	struct Vector2;
	struct Matrix2;
	struct Vector3;
	struct Matrix3;

	struct Vector2
	{
		Vector2(const float& newX = 0, const float& newY = 0)
		{
			x = newX;
			y = newY;
		}

		float x;
		float y;

		Vector3 Extend() const;

		Vector2 Normalize() const;

		const static Vector2 origin;
	};

	struct Matrix2
	{
		Matrix2(const float& newA, const float& newB, const float& newC, const float& newD)
		{
			a = newA;
			b = newB;
			c = newC;
			d = newD;
		}

		float a;
		float b;
		float c;
		float d;

		const static Matrix2 Identity;

		Matrix3 Extend();
	};

	Matrix2 operator*(const Matrix2& first, const Matrix2& second);
	Vector2 operator*(const Matrix2& matrix, const Vector2& vector);

	Vector2 operator+(const Vector2& first, const Vector2& second);
	Vector2 operator-(const Vector2& first, const Vector2& second);

	struct Vector3
	{
		Vector3(const float& newX = 0, const float& newY = 0, const float& newZ = 0)
		{
			x = newX;
			y = newY;
			z = newZ;
		}

		float x;
		float y;
		float z;

		Vector3 Normalize() const;
	};

	struct Matrix3
	{
		Matrix3(
			const float& newA, const float& newB, const float& newC,
			const float& newD, const float& newE, const float& newF,
			const float& newG, const float& newH, const float& newI)
		{
			a = newA;
			b = newB;
			c = newC;
			d = newD;
			e = newE;
			f = newF;
			g = newG;
			h = newH;
			i = newI;
		}

		float a;
		float b;
		float c;
		float d;
		float e;
		float f;
		float g;
		float h;
		float i;

		const static Matrix3 Identity;
	};

	Matrix3 operator*(const Matrix3& first, const Matrix3& second);
	Vector3 operator*(const Matrix3& matrix, const Vector3& vector);

	Vector3 operator+(const Vector3& first, const Vector3& second);
	Vector3 operator-(const Vector3& first, const Vector3& second);

	Matrix2 RotationMatrix(const float& angle);
	Matrix3 ScaleMatrix(const float& scale);

	float CalculateAngle(const Vector2& first, const Vector2& second);
}