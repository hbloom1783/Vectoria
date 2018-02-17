#include "Glitching.h"

#include <stdint.h>
#include <math.h>

#include <random>
using std::random_device;
using std::default_random_engine;
using std::uniform_real_distribution;
using std::normal_distribution;
using std::poisson_distribution;

#include <algorithm>
using std::shuffle;
using std::begin;

namespace RNG
{
	#pragma region Loose Functions

	float ReadRD()
	{
		static random_device rd;

		return rd();
	}

	default_random_engine& GetEngine()
	{
		static default_random_engine engine(ReadRD());

		return engine;
	}

	float Clamp(float input, float bound)
	{
		if (input > bound)
			return bound;
		else if (input < -bound)
			return -bound;
		else
			return input;
	}

	float Clamp(float input, float upper, float lower)
	{
		if (input > upper)
			return upper;
		else if (input < lower)
			return lower;
		else
			return input;
	}

	float RangeUniform(float bound)
	{
		uniform_real_distribution<float> dist(-bound, bound);
		return dist(GetEngine());
	}

	float RangeUniform(float lower, float upper)
	{
		uniform_real_distribution<float> dist(lower, upper);
		return dist(GetEngine());
	}

	float RangeNormal(float bound)
	{
		if (bound == 0) return 0.0f;

		normal_distribution<float> dist(0.0f, bound / 3);
		return Clamp(dist(GetEngine()), bound);
	}

	float RangeNormal(float mean, float sigma)
	{
		if (sigma == 0) return mean;

		normal_distribution<float> dist(mean, sigma);
		return dist(GetEngine());
	}

	int CountPoisson(int input)
	{
		poisson_distribution<int> dist(input);
		return dist(GetEngine());
	}

	bool CoinFlip(float oddsOfTrue)
	{
		if (RangeUniform(0, 1) <= oddsOfTrue) return true;
		else return false;
	}

	#pragma endregion

	#pragma region Perlin

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

	Perlin::Perlin()
	{
		this->Reseed(ReadRD());
	}

	Perlin::Perlin(uint32_t seed)
	{
		this->Reseed(seed);
	}

	void Perlin::Reseed(uint32_t seed)
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

	float Perlin::Noise(float x) const
	{
		return this->Noise(x, 0.0, 0.0);
	}

	float Perlin::Noise(float x, float y) const
	{
		return this->Noise(x, y, 0.0);
	}

	float Perlin::Noise(float x, float y, float z) const
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

		int32_t A = p[X] + Y;
		int32_t AA = p[A] + Z;
		int32_t AB = p[A + 1] + Z;

		int32_t B = p[X + 1] + Y;
		int32_t BA = p[B] + Z;
		int32_t BB = p[B + 1] + Z;

		return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z),
			Grad(p[BA], x - 1, y, z)),
			Lerp(u, Grad(p[AB], x, y - 1, z),
				Grad(p[BB], x - 1, y - 1, z))),
			Lerp(v, Lerp(u, Grad(p[AA + 1], x, y, z - 1),
				Grad(p[BA + 1], x - 1, y, z - 1)),
				Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1),
					Grad(p[BB + 1], x - 1, y - 1, z - 1))));
	}

	float Perlin::Noise(const Vector2& input) const
	{
		// Offsetting by pi helps avoid the whole-number dead zone
		return this->Noise(
			input.x + std::_Pi,
			input.y + std::_Pi);
	}

	float Perlin::Noise(const Vector3& input) const
	{
		// Offsetting by pi helps avoid the whole-number dead zone
		return this->Noise(
			input.x + std::_Pi,
			input.y + std::_Pi,
			input.z + std::_Pi);
	}

	float Perlin::OctaveNoise(float x, int32_t octaves) const
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

	float Perlin::OctaveNoise(float x, float y, int32_t octaves) const
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

	float Perlin::OctaveNoise(float x, float y, float z, int32_t octaves) const
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

	float Perlin::OctaveNoise(const Vector2& input, int32_t octaves) const
	{
		// Offsetting by pi helps avoid the whole-number dead zone
		return this->OctaveNoise(
			input.x + std::_Pi,
			input.y + std::_Pi,
			octaves);
	}

	float Perlin::OctaveNoise(const Vector3& input, int32_t octaves) const
	{
		// Offsetting by pi helps avoid the whole-number dead zone
		return this->OctaveNoise(
			input.x + std::_Pi,
			input.y + std::_Pi,
			input.z + std::_Pi,
			octaves);
	}

	#pragma endregion
}

namespace Models
{
	#pragma region DistortionMap

	DistortionMap::DistortionMap(float bound) : xNoise(RNG::ReadRD()), yNoise(RNG::ReadRD()), zNoise(RNG::ReadRD())
	{
		this->bound = bound;
	}

	float DistortionMap::Sample(const Vector3& input, const RNG::Perlin& source) const
	{
		if (this->bound == 0)
		{
			return 0;
		}
		else
		{
			return source.Noise(input) * this->bound;
		}
	}

	Vector2 DistortionMap::Distort(const Vector2& orig, float z) const
	{
		return this->Distort(orig.Extend(z)).Project() + orig;
	}

	Vector3 DistortionMap::Distort(const Vector3& orig) const
	{
		return Vector3(
			this->Sample(orig, xNoise),
			this->Sample(orig, yNoise),
			this->Sample(orig, zNoise)) + orig;
	}

	LineSegment DistortionMap::Distort(const LineSegment& lineSegment) const
	{
		Vector3 p0 = Distort(lineSegment.GetP0());
		Vector3 p1 = Distort(lineSegment.GetP1());

		return LineSegment(p0, p1);
	}

	LineSegment DistortionMap::Distort(const LineSegment& lineSegment, float z) const
	{
		Vector2 p0 = Distort(lineSegment.GetP0().Project(), z);
		Vector2 p1 = Distort(lineSegment.GetP1().Project(), z);

		return LineSegment(p0.Extend(), p1.Extend());
	}

	Triangle DistortionMap::Distort(const Triangle& triangle) const
	{
		Vector3 p0 = Distort(triangle.GetP0());
		Vector3 p1 = Distort(triangle.GetP1());
		Vector3 p2 = Distort(triangle.GetP2());

		return Triangle(p0, p1, p2);
	}

	Triangle DistortionMap::Distort(const Triangle& triangle, float z) const
	{
		Vector2 p0 = Distort(triangle.GetP0().Project(), z);
		Vector2 p1 = Distort(triangle.GetP1().Project(), z);
		Vector2 p2 = Distort(triangle.GetP2().Project(), z);

		return Triangle(p0.Extend(), p1.Extend(), p2.Extend());
	}

	#pragma endregion
}