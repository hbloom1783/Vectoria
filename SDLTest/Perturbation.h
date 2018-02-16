#pragma once

//#include <math.h>

#include <stdint.h>

#include "Matrix.h"
using Geometry::Vector2;
using Geometry::Vector3;

namespace Perturb
{
	float PerturbUniform(float input);
	float PerturbNormal(float input);
	int PerturbPoisson(int input);

	const static int frequencyCount = 256;
	const static int dimensionCount = 3;

	class PerlinNoise
	{
	public:
		explicit PerlinNoise(uint32_t seed);

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

	class PerturbPerlin
	{
	public:
		PerturbPerlin(float bound);
		Vector2 PerturbVector(Vector2 input) const;
		Vector3 PerturbVector(Vector3 input) const;

	private:
		float bound;
		PerlinNoise xNoise;
		PerlinNoise yNoise;
		PerlinNoise zNoise;
	};
}
