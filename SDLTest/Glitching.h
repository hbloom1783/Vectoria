#pragma once

#include <stdint.h>

#include "Matrix.h"
using Geometry::Vector2;
using Geometry::Vector3;

#include "Triangle.h"
using Geometry::Triangle;

#include "LineSegment.h"
using Geometry::LineSegment;

#include "Color.h"
using Color::HSVAColor;

namespace Models
{
	float RangeUniform(float bound);
	float RangeNormal(float bound);
	int CountPoisson(int expected);
	bool CoinFlip(float oddsOfTrue = 0.50);

	typedef HSVAColor(*HSVAColorMutator)(const HSVAColor&);
	typedef LineSegment(*LineSegmentMutator)(const LineSegment&);
	typedef Triangle(*TriangleMutator)(const Triangle&);

	class Perlin
	{
	public:
		explicit Perlin(uint32_t seed);

		void Reseed(uint32_t seed);

		float Noise(float x) const;
		float Noise(float x, float y) const;
		float Noise(float x, float y, float z) const;
		float Noise(const Vector2& input) const;
		float Noise(const Vector3& input) const;

		float OctaveNoise(float x, int32_t octaves = 1) const;
		float OctaveNoise(float x, float y, int32_t octaves = 1) const;
		float OctaveNoise(float x, float y, float z, int32_t octaves = 1) const;
		float OctaveNoise(const Vector2& input, int32_t octaves = 1) const;
		float OctaveNoise(const Vector3& input, int32_t octaves = 1) const;

	private:
		int32_t p[512];
	};

	class DistortionMap
	{
	public:
		DistortionMap(float bound = 0.0f);

		Vector2 Distort(const Vector2& orig, float z = 0.0f) const;
		Vector3 Distort(const Vector3& orig) const;

		LineSegment Distort(const LineSegment& lineSegment) const;
		LineSegment Distort(const LineSegment& lineSegment, float z) const;
		Triangle Distort(const Triangle& triangle) const;
		Triangle Distort(const Triangle& triangle, float z) const;

	private:
		float bound;

		float Sample(const Vector3& input, const Perlin& source) const;

		Perlin xNoise;
		Perlin yNoise;
		Perlin zNoise;
	};
}
