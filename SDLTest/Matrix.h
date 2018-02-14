#pragma once

namespace Geometry
{
	struct Vector2;
	struct Matrix2;
	struct Vector3;
	struct Matrix3;

	struct Vector2
	{
		Vector2(float newX = 0, float newY = 0)
		{
			x = newX;
			y = newY;
		}

		float x;
		float y;

		Vector3 Extend() const;

		Vector2 Normalize() const;

		const static Vector2 Origin;
	};

	struct Matrix2
	{
		Matrix2(float newA, float newB, float newC, float newD)
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

		Matrix3 Extend();

		static Matrix2 RotationMatrix(const float& angle);

		const static Matrix2 Identity;
	};

	struct Vector3
	{
		Vector3(float newX = 0, float newY = 0, float newZ = 0)
		{
			x = newX;
			y = newY;
			z = newZ;
		}

		float x;
		float y;
		float z;

		Vector3 Normalize() const;

		Vector2 Project() const;

		const static Vector3 Origin;
	};

	struct Matrix3
	{
		Matrix3(
			float newA, float newB, float newC,
			float newD, float newE, float newF,
			float newG, float newH, float newI)
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

		static Matrix3 ScaleMatrix(const float& scale);

		const static Matrix3 Identity;
	};

	Matrix2 operator*(const Matrix2& first, const Matrix2& second);
	Vector2 operator*(const Matrix2& matrix, const Vector2& vector);
	Vector2 operator*(float coefficient, const Vector2& vector);
	Vector2 operator*(const Vector2& vector, float coefficient);

	Vector2 operator+(const Vector2& first, const Vector2& second);
	Vector2 operator-(const Vector2& first, const Vector2& second);

	Matrix3 operator*(const Matrix3& first, const Matrix3& second);
	Vector3 operator*(const Matrix3& matrix, const Vector3& vector);
	Vector3 operator*(float coefficient, const Vector3& vector);
	Vector3 operator*(const Vector3& vector, float coefficient);

	Vector3 operator+(const Vector3& first, const Vector3& second);
	Vector3 operator-(const Vector3& first, const Vector3& second);

	float CalculateAngle(const Vector2& first, const Vector2& second);
}