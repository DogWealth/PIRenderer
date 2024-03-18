#pragma once
#include "Vector.h"
#include "Matrix.h"
#include <math.h>
#define PI 3.141592653589793
#define PI2 6.283185307179586
#define NUM_RINGS 10
#define NUM_SAMPLES 100
namespace PIRenderer {
	inline float rand_2to1(Vector2 uv) {
		// 0 - 1
		const float a = 12.9898, b = 78.233, c = 43758.5453;
		float dt = uv * Vector2(a, b);
		float sn = dt - PI * floor(dt / PI);
		return sin(sn) * c - floor(sin(sn) * c);
	}

	inline Vector2 poissonDisk[NUM_SAMPLES];

	inline void poissonDiskSamples(const Vector2& randomSeed, int numSamples) {

		float ANGLE_STEP = PI2 * float(NUM_RINGS) / float(numSamples);
		float INV_NUM_SAMPLES = 1.0 / float(numSamples);

		float angle = rand_2to1(randomSeed) * PI2;
		float radius = INV_NUM_SAMPLES;
		float radiusStep = radius;

		for (int i = 0; i < numSamples; i++) {
			poissonDisk[i] = Vector2(cos(angle), sin(angle)) * pow(radius, 0.75);
			radius += radiusStep;
			angle += ANGLE_STEP;
		}
	}

	inline void GetSummedAreaTable(std::vector<std::vector<double>>* SAT, float* nums, int w, int h)
	{
		(*SAT)[0][0] = nums[0];
		for (int i = 1; i < w; i++)
		{
			(*SAT)[0][i] = (*SAT)[0][i-1] + nums[i]; //初始化第一行
		}

		for (int i = 1; i < h; i++)
		{ 
			(*SAT)[i][0] = (*SAT)[i-1][0] + nums[i * w];//初始化第一列
		}

		for (int i = 1; i < h; i++)
		{
			for (int j = 1; j < w; j++)
			{
				(*SAT)[i][j] = (*SAT)[i - 1][j] + (*SAT)[i][j - 1] + nums[i * w + j] - (*SAT)[i - 1][j - 1];
			}
		}
	}
}