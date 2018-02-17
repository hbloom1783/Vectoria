#include "Perturbation.h"

#include <stdint.h>

#include <random>
using std::random_device;
using std::default_random_engine;
using std::uniform_real_distribution;
using std::normal_distribution;
using std::poisson_distribution;
using std::_Pi;
using std::begin;

#include <algorithm>
using std::shuffle;

#include <assert.h>

namespace Perturb
{
	#pragma region Loose Functions

	random_device& GetDevice()
	{
		static random_device rd;

		return rd;
	}

	int QueryDevice()
	{
		static random_device rd;

		return rd();
	}

	default_random_engine& GetEngine()
	{
		static default_random_engine engine(QueryDevice());

		return engine;
	}

	float PerturbUniform(float input)
	{
		uniform_real_distribution<float> dist(-input, input);
		return dist(GetEngine());
	}

	float PerturbNormal(float input)
	{
		if (input == 0) return 0;

		normal_distribution<float> dist(0.0f, input / 3);
		return dist(GetEngine());
	}

	int PerturbPoisson(int input)
	{
		poisson_distribution<int> dist(input);
		return dist(GetEngine());
	}

	#pragma endregion

	#pragma region PerlinNoise

	double Fade(double t) noexcept
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	double Lerp(double t, double a, double b) noexcept
	{
		return a + t * (b - a);
	}

	double Grad(int32_t hash, double x, double y, double z) noexcept
	{
		const int32_t h = hash & 15;
		const double u = h < 8 ? x : y;
		const double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}

	PerlinNoise::PerlinNoise(uint32_t seed)
	{
		this->Reseed(seed);
	}

	void PerlinNoise::Reseed(uint32_t seed)
	{
		for (size_t i = 0; i < 256; ++i)
		{
			p[i] = i;
		}

		shuffle(begin(this->p), begin(this->p) + 256, default_random_engine(seed));

		for (size_t i = 0; i < 256; ++i)
		{
			p[256 + i] = p[i];
		}
	}

	float PerlinNoise::Noise(float x) const
	{
		return this->Noise(x, 0.0, 0.0);
	}

	float PerlinNoise::Noise(float x, float y) const
	{
		return this->Noise(x, y, 0.0);
	}

	float PerlinNoise::Noise(float x, float y, float z) const
	{

		int32_t X = static_cast<int32_t>(floor(x)) & 255;
		int32_t Y = static_cast<int32_t>(floor(y)) & 255;
		int32_t Z = static_cast<int32_t>(floor(z)) & 255;

		x -= floor(x);
		y -= floor(y);
		z -= floor(z);

		double u = Fade(x);
		double v = Fade(y);
		double w = Fade(z);

		int32_t A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
		int32_t B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

		return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z),
			Grad(p[BA], x - 1, y, z)),
			Lerp(u, Grad(p[AB], x, y - 1, z),
				Grad(p[BB], x - 1, y - 1, z))),
			Lerp(v, Lerp(u, Grad(p[AA + 1], x, y, z - 1),
				Grad(p[BA + 1], x - 1, y, z - 1)),
				Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1),
					Grad(p[BB + 1], x - 1, y - 1, z - 1))));
	}

	float PerlinNoise::Noise(const Vector2& input) const
	{
		// Offsetting by pi helps avoid the whole-number dead zone
		return this->Noise(
			input.x + _Pi,
			input.y + _Pi);
	}

	float PerlinNoise::Noise(const Vector3& input) const
	{
		// Offsetting by pi helps avoid the whole-number dead zone
		return this->Noise(
			input.x + _Pi,
			input.y + _Pi,
			input.z + _Pi);
	}

	float PerlinNoise::OctaveNoise(float x, int32_t octaves) const
	{
		double result = 0.0;
		double amp = 1.0;

		for (int32_t i = 0; i < octaves; ++i)
		{
			result += this->Noise(x) * amp;
			x *= 2.0;
			amp *= 0.5;
		}

		return result;
	}

	float PerlinNoise::OctaveNoise(float x, float y, int32_t octaves) const
	{
		double result = 0.0;
		double amp = 1.0;

		for (int32_t i = 0; i < octaves; ++i)
		{
			result += this->Noise(x, y) * amp;
			x *= 2.0;
			y *= 2.0;
			amp *= 0.5;
		}

		return result;
	}

	float PerlinNoise::OctaveNoise(float x, float y, float z, int32_t octaves) const
	{
		double result = 0.0;
		double amp = 1.0;

		for (int32_t i = 0; i < octaves; ++i)
		{
			result += this->Noise(x, y, z) * amp;
			x *= 2.0;
			y *= 2.0;
			z *= 2.0;
			amp *= 0.5;
		}

		return result;
	}

	float PerlinNoise::OctaveNoise(const Vector2& input, int32_t octaves) const
	{
		// Offsetting by pi helps avoid the whole-number dead zone
		return this->OctaveNoise(
			input.x + _Pi,
			input.y + _Pi,
			octaves);
	}

	float PerlinNoise::OctaveNoise(const Vector3& input, int32_t octaves) const
	{
		// Offsetting by pi helps avoid the whole-number dead zone
		return this->OctaveNoise(
			input.x + _Pi,
			input.y + _Pi,
			input.z + _Pi,
			octaves);
	}

	#pragma endregion

	#pragma region PerturbPerlin

	PerturbPerlin::PerturbPerlin(float bound) : xNoise(QueryDevice()), yNoise(QueryDevice()), zNoise(QueryDevice())
	{
		this->bound = bound;
	}

	Vector2 PerturbPerlin::PerturbVector(Vector2 input, float scale) const
	{
		return this->PerturbVector(input.Extend(), scale).Project();
	}

	Vector3 PerturbPerlin::PerturbVector(Vector3 input, float scale) const
	{
		Vector3 offset = Vector3(
			this->xNoise.OctaveNoise(input, 1) * this->bound / scale,
			this->yNoise.OctaveNoise(input, 1) * this->bound / scale,
			this->zNoise.OctaveNoise(input, 1) * this->bound / scale);

		return input + offset;
	}

	#pragma endregion
}