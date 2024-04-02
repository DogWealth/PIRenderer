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

	inline Matrix4 GetTBN(const V2F& v1, const V2F& v2, const V2F& v3)
	{
		Vector3f edge1 = v2.m_WorldPos - v1.m_WorldPos;
		Vector3f edge2 = v3.m_WorldPos - v1.m_WorldPos;

		Vector2 deltaUV1 = v2.m_Texcoord - v1.m_Texcoord;
		Vector2 deltaUV2 = v3.m_Texcoord - v1.m_Texcoord;

		float f = 1.0f / (deltaUV1.u * deltaUV2.v - deltaUV2.u * deltaUV1.v);

		Vector3f tangent, bitangent, normal;
		tangent.x = f * (deltaUV2.v * edge1.x - deltaUV1.v * edge2.x);
		tangent.y = f * (deltaUV2.v * edge1.y - deltaUV1.v * edge2.y);
		tangent.z = f * (deltaUV2.v * edge1.z - deltaUV1.v * edge2.z);
		tangent.Normalize();

		bitangent.x = f * (-deltaUV2.u * edge1.x + deltaUV1.u * edge2.x);
		bitangent.y = f * (-deltaUV2.u * edge1.y + deltaUV1.u * edge2.y);
		bitangent.z = f * (-deltaUV2.u * edge1.z + deltaUV1.u * edge2.z);
		bitangent.Normalize();

		normal = Vector3f::CrossProduct(tangent, bitangent);
		normal.Normalize();

		Matrix4 TBN;
		TBN.m_Mat[0][0] = tangent.x, TBN.m_Mat[0][1] = bitangent.x, TBN.m_Mat[0][2] = normal.x;
		TBN.m_Mat[1][0] = tangent.y, TBN.m_Mat[1][1] = bitangent.y, TBN.m_Mat[1][2] = normal.y;
		TBN.m_Mat[2][0] = tangent.z, TBN.m_Mat[2][1] = bitangent.z, TBN.m_Mat[2][2] = normal.z;

		TBN.m_Mat[0][3] = 0, TBN.m_Mat[1][3] = 0, TBN.m_Mat[2][3] = 0;
		TBN.m_Mat[3][0] = 0, TBN.m_Mat[3][1] = 0, TBN.m_Mat[3][2] = 0;
		TBN.m_Mat[3][3] = 1;

		return TBN;
	}

	inline Vector3f GetTangent(const V2F& v1, const V2F& v2, const V2F& v3)
	{
		Vector3f edge1 = v2.m_WorldPos - v1.m_WorldPos;
		Vector3f edge2 = v3.m_WorldPos - v1.m_WorldPos;

		Vector2 deltaUV1 = v2.m_Texcoord - v1.m_Texcoord;
		Vector2 deltaUV2 = v3.m_Texcoord - v1.m_Texcoord;

		float f = 1.0f / (deltaUV1.u * deltaUV2.v - deltaUV2.u * deltaUV1.v);

		Vector3f tangent;
		tangent.x = f * (deltaUV2.v * edge1.x - deltaUV1.v * edge2.x);
		tangent.y = f * (deltaUV2.v * edge1.y - deltaUV1.v * edge2.y);
		tangent.z = f * (deltaUV2.v * edge1.z - deltaUV1.v * edge2.z);
		tangent.Normalize();

		return tangent;
	}

	inline Matrix4 GetTBN(const Vector3f& tangent, const Vector3f& bitangent, const Vector3f& normal)
	{
		Matrix4 TBN;
		TBN.m_Mat[0][0] = tangent.x, TBN.m_Mat[0][1] = bitangent.x, TBN.m_Mat[0][2] = normal.x;
		TBN.m_Mat[1][0] = tangent.y, TBN.m_Mat[1][1] = bitangent.y, TBN.m_Mat[1][2] = normal.y;
		TBN.m_Mat[2][0] = tangent.z, TBN.m_Mat[2][1] = bitangent.z, TBN.m_Mat[2][2] = normal.z;

		TBN.m_Mat[0][3] = 0, TBN.m_Mat[1][3] = 0, TBN.m_Mat[2][3] = 0;
		TBN.m_Mat[3][0] = 0, TBN.m_Mat[3][1] = 0, TBN.m_Mat[3][2] = 0;
		TBN.m_Mat[3][3] = 1;

		return TBN;
	}

	inline Vector3f Max(const Vector3f& v1, const Vector3f& v2)
	{
		return Vector3f(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
	}

	inline float RadicalInverse_VdC(uint32_t bits)
	{
		bits = (bits << 16u) | (bits >> 16u);
		bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
		bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
		bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
		bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
		return float(bits) * 2.3283064365386963e-10; // / 0x100000000
	}

	inline Vector2 Hammersley(uint32_t i, uint32_t N)
	{
		return Vector2(float(i) / float(N), RadicalInverse_VdC(i));
	}

	inline Vector3f ImportanceSampleGGX(Vector2 Xi, Vector3f N, float roughness)
	{
		float a = roughness * roughness;

		float phi = 2.0 * PI * Xi.u;
		float cosTheta = sqrt((1.0 - Xi.v) / (1.0 + (a * a - 1.0) * Xi.v));
		float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		// from spherical coordinates to cartesian coordinates
		Vector3f H;
		H.x = cos(phi) * sinTheta;
		H.y = sin(phi) * sinTheta;
		H.z = cosTheta;

		// from tangent-space vector to world-space sample vector
		Vector3f up = abs(N.z) < 0.999 ? Vector3f(0.0, 0.0, 1.0) : Vector3f(1.0, 0.0, 0.0);
		Vector3f tangent = Vector3f::Normalize(Vector3f::CrossProduct(up, N));
		Vector3f bitangent = Vector3f::CrossProduct(N, tangent);

		Vector3f sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
		return Vector3f::Normalize(sampleVec);
	}
}