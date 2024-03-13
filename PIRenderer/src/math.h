#pragma once
#include "Vector.h"
#include "Matrix.h"
#include <math.h>
#define PI 3.141592653589793
#define PI2 6.283185307179586
#define NUM_RINGS 10
#define NUM_SAMPLES 100
namespace PIRenderer {
	float rand_2to1(Vector2 uv) {
		// 0 - 1
		const float a = 12.9898, b = 78.233, c = 43758.5453;
		float dt = uv * Vector2(a, b);
		float sn = dt - PI * floor(dt / PI);
		return sin(sn) * c - floor(sin(sn) * c);
	}

	Vector2 poissonDisk[NUM_SAMPLES];

	void poissonDiskSamples(const Vector2& randomSeed, int numSamples) {

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
}