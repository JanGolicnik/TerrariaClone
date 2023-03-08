#include "noise.h"
#ifndef M_PI 
#define M_PI 3.14159265358979323846
#endif

#include <utils.h>

namespace noise {
	
	int mini;
	int maxi;

	std::vector<float> generate(int lo, int hi, int precision, int seed, int length)
	{
		srand(seed);
		mini = lo;
		maxi = hi;
		lo *= precision;
		hi *= precision;
		std::vector<float> values;
		for (int i = 0; i < length; i++) {
			float val1 = ((rand() % (hi - lo) + lo) / (float)precision);
			values.push_back(val1);
		}
		srand(time(NULL));
		return values;
	}

	float sample(float t, std::vector<float>* values)
	{
		int lo = floor(t);
		int hi = lo + 1;
		t = t - lo;
		
		if (t < 0) {
			t = values->size() - t;
		}
		if (t > values->size()) {
			int tmp = (int)t % values->size();
			t = tmp + t - lo;
		}

		return (*values)[lo] * (1 - t) + (*values)[hi] * t;
	}

	float sampleCosine(float t, std::vector<float>* values)
	{
		t *= values->size();
		int lo = floor(t);
		int hi = lo + 1;
		t = t - lo;
		t = (1 + cos(t * M_PI)) * 0.5f;

		if (hi > values->size()-1) {
			hi = hi % (values->size()-1);
		}
		
		return (*values)[lo] * (t) + (*values)[hi] * (1 - t);

	}

	float sampleSmoothstep(float t, std::vector<float>* values)
	{
		int lo = floor(t);
		int hi = lo + 1;
		t = t - lo;
		t = t * t * (3 - 2 * t);
		
		if (t < 0) {
			t = values->size() - t;
		}
		if (t > values->size()) {
			int tmp = (int)t % values->size();
			t = tmp + t - lo;
		}

		return (*values)[lo] * (1 - t) + (*values)[hi] * t;
	}

}
