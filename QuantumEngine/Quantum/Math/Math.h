#pragma once

#include <DirectXMath.h>



namespace Quantum
{
	namespace Math
	{
		float clamp(float x, float min, float max);

		int randomInt(int min, int max);

		float randomFloat(float min = 0.0F, float max = 1.0F);
	}
}
