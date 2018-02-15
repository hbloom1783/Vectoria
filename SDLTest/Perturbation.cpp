#include "Perturbation.h"

#include <random>
using std::random_device;
using std::default_random_engine;
using std::uniform_real_distribution;
using std::normal_distribution;
using std::poisson_distribution;

namespace Perturb
{
	default_random_engine& GetEngine()
	{
		static random_device rd;
		static default_random_engine engine(rd());

		return engine;
	}

	float PerturbUniform(float input)
	{
		return 0;
	}

	float PerturbStandard(float input)
	{
		return 0;
	}

	int PerturbPoisson(int input)
	{
		return 0;
	}
}