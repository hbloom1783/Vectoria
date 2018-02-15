#pragma once

#include <math.h>

#include "Matrix.h"
using Geometry::Vector2;
using Geometry::Vector3;

#include "PerlinNoise.h"

namespace Perturb
{
	float PerturbUniform(float input);
	float PerturbStandard(float input);
	int PerturbPoisson(int input);

	const static int frequencyCount = 256;
	const static int dimensionCount = 3;

	class PerturbPerlin
	{
	public:
		PerturbPerlin(float bound);
		Vector2 PerturbVector(Vector2 input);
		Vector3 PerturbVector(Vector3 input);

	private:
		float bound;
		siv::PerlinNoise xNoise;
		siv::PerlinNoise yNoise;
		siv::PerlinNoise zNoise;
	};
}
