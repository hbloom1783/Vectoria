#include "Perturbation.h"

#include <random>
using std::random_device;
using std::default_random_engine;
using std::uniform_real_distribution;
using std::normal_distribution;
using std::poisson_distribution;
using std::_Pi;

#include <assert.h>

#include "PerlinNoise.h"

namespace Perturb
{
	#pragma region Loose Functions

	random_device& GetDevice()
	{
		static random_device rd;

		return rd;
	}

	default_random_engine& GetEngine()
	{
		static default_random_engine engine(GetDevice()());

		return engine;
	}

	float PerturbUniform(float input)
	{
		uniform_real_distribution<float> dist(-input, input);
		return dist(GetEngine());
	}

	float PerturbStandard(float input)
	{
		if (input = 0) return 0;

		normal_distribution<float> dist(0.0f, input / 3);
		return dist(GetEngine());
	}

	int PerturbPoisson(int input)
	{
		poisson_distribution<int> dist(input);
		return dist(GetEngine());
	}

	#pragma endregion

	#pragma region PerturbPerlin

	PerturbPerlin::PerturbPerlin(float bound) : xNoise(GetDevice()()), yNoise(GetDevice()()), zNoise(GetDevice()())
	{
		this->bound = bound;
	}

	Vector2 PerturbPerlin::PerturbVector(Vector2 input)
	{
		return this->PerturbVector(input.Extend()).Project();
	}

	Vector3 PerturbPerlin::PerturbVector(Vector3 input)
	{
		Vector3 offset = Vector3(
			this->xNoise.noise(input.x + _Pi, input.y + _Pi, input.z + _Pi) * this->bound,
			this->yNoise.noise(input.x + _Pi, input.y + _Pi, input.z + _Pi) * this->bound,
			this->zNoise.noise(input.x + _Pi, input.y + _Pi, input.z + _Pi) * this->bound);

		return input + offset;
	}

	#pragma endregion
}