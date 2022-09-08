#pragma once

#include <core.h>

namespace world
{
	extern int mini;
	extern int maxi;

	std::vector<float> generate(int lo, int high, int precision, int seed, int length);
	float sample(float t, std::vector<float>* values);
	float sampleCosine(float t, std::vector<float> *values);
	float sampleSmoothstep(float t, std::vector<float> *values);
};

