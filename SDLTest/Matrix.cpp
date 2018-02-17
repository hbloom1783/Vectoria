#include "Matrix.h"

#define _USE_MATH_DEFINES
#include <cmath>

namespace Geometry
{

	#pragma region Vector2

	Vector3 Vector2::Extend(float z) const
	{
		return Vector3(this->x, this->y, z);
	}

	Vector2 Vector2::Normalize() const
	{
		float nFactor = 1.0f / sqrtf((this->x * this->x) + (this->y * this->y));
		return Vector2(this->x * nFactor, this->y * nFactor);
	}

	const Vector2 Vector2::Origin = Vector2(0, 0);

	#pragma endregion

	#pragma region Matrix2

	Matrix3 Matrix2::Extend()
	{
		return Matrix3(
			this->a, this->b, 0,
			this->c, this->d, 0,
			0, 0, 1);
	}

	Matrix2 Matrix2::RotationMatrix(const float& angle)
	{
		float radians = angle * (float)(M_PI / 180.0);
		return Matrix2(
			cos(radians), -sin(radians),
			sin(radians), cos(radians));
	}

	const Matrix2 Matrix2::Identity = Matrix2(1, 0, 0, 1);

	#pragma endregion

	#pragma region Vector3

	Vector3 Vector3::Normalize() const
	{
		float nFactor = 1.0f / sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
		return Vector3(this->x * nFactor, this->y * nFactor, this->z * nFactor);
	}

	Vector2 Vector3::Project() const
	{
		return Vector2(this->x, this->y);
	}

	const Vector3 Vector3::Origin = Vector3(0, 0, 0);

	#pragma endregion

	#pragma region Matrix3

	Matrix3 Matrix3::ScaleMatrix(const float& scale)
	{
		return Matrix3(
			scale, 0, 0,
			0, scale, 0,
			0, 0, scale);
	}

	const Matrix3 Matrix3::Identity = Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1);

	#pragma endregion

	#pragma region Operators

	Matrix2 operator*(const Matrix2& first, const Matrix2& second)
	{
		//      a b
		//      c d
		//
		// a b  a b
		// c d  c d

		float a = (first.a * second.a) + (first.b * second.c);
		float b = (first.a * second.b) + (first.b * second.d);
		float c = (first.c * second.a) + (first.d * second.c);
		float d = (first.c * second.b) + (first.d * second.d);

		return Matrix2(a, b, c, d);
	}

	Vector2 operator*(const Matrix2& matrix, const Vector2& vector)
	{
		//      x
		//      y
		//
		// a b  x
		// c d  y

		float x = (matrix.a * vector.x) + (matrix.b * vector.y);
		float y = (matrix.c * vector.x) + (matrix.d * vector.y);

		return Vector2(x, y);
	}

	Vector2 operator*(float coeff, const Vector2& vector)
	{
		return Vector2(vector.x * coeff, vector.y * coeff);
	}

	Vector2 operator*(const Vector2& vector, float coeff)
	{
		return coeff * vector;
	}

	Vector2 operator+(const Vector2& first, const Vector2& second)
	{
		return Vector2(first.x + second.x, first.y + second.y);
	}

	Vector2 operator-(const Vector2& first, const Vector2& second)
	{
		return first + Vector2(-second.x, -second.y);
	}

	Matrix3 operator*(const Matrix3& first, const Matrix3& second)
	{
		//        a b c
		//        d e f
		//        g h i
		//
		// a b c  a b c
		// d e f  d e f
		// g h i  g h i

		float a = (first.a * second.a) + (first.b * second.d) + (first.c * second.g);
		float b = (first.a * second.b) + (first.b * second.e) + (first.c * second.h);
		float c = (first.a * second.c) + (first.b * second.f) + (first.c * second.i);

		float d = (first.d * second.a) + (first.e * second.d) + (first.f * second.g);
		float e = (first.d * second.b) + (first.e * second.e) + (first.f * second.h);
		float f = (first.d * second.c) + (first.e * second.f) + (first.f * second.i);

		float g = (first.g * second.a) + (first.h * second.d) + (first.i * second.g);
		float h = (first.g * second.b) + (first.h * second.e) + (first.i * second.h);
		float i = (first.g * second.c) + (first.h * second.f) + (first.i * second.i);

		return Matrix3(a, b, c, d, e, f, g, h, i);
	}

	Vector3 operator*(const Matrix3& matrix, const Vector3& vector)
	{
		//        x
		//        y
		//        z
		//
		// a b c  x
		// d e f  y
		// g h i  z

		float x = (matrix.a * vector.x) + (matrix.b * vector.y) + (matrix.c * vector.z);
		float y = (matrix.d * vector.x) + (matrix.e * vector.y) + (matrix.f * vector.z);
		float z = (matrix.g * vector.x) + (matrix.h * vector.y) + (matrix.i * vector.z);

		return Vector3(x, y, z);
	}

	Vector3 operator*(float coeff, const Vector3& vector)
	{
		return Vector3(vector.x * coeff, vector.y * coeff, vector.z * coeff);
	}

	Vector3 operator*(const Vector3& vector, float coeff)
	{
		return coeff * vector;
	}

	Vector3 operator+(const Vector3& first, const Vector3& second)
	{
		return Vector3(first.x + second.x, first.y + second.y, first.z + second.z);
	}

	Vector3 operator-(const Vector3& first, const Vector3& second)
	{
		return first + Vector3(-second.x, -second.y, -second.z);
	}

	#pragma endregion

	float CalculateAngle(const Vector2& first, const Vector2& second)
	{
		Vector2 n1 = first.Normalize();
		Vector2 n2 = second.Normalize();
		return acos((n1.x * n2.x) + (n1.y * n2.y)) * (180.0f / M_PI);
	}
}