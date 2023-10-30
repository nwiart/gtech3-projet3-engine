#include "Math.h"

#include <stdlib.h>



int Quantum::Math::randomInt(int min, int max)
{
	return rand() % (max - min) + min;
}

float Quantum::Math::randomFloat(float min, float max)
{
	return (rand() / (float) RAND_MAX) * (max - min) + min;
}
