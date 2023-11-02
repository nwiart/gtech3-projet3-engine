#include "Math.h"

#include <stdlib.h>



float Quantum::Math::clamp(float x, float min, float max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

int Quantum::Math::randomInt(int min, int max)
{
	return rand() % (max - min) + min;
}

float Quantum::Math::randomFloat(float min, float max)
{
	return (rand() / (float) RAND_MAX) * (max - min) + min;
}
