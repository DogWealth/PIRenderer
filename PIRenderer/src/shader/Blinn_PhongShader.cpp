#include "Blinn_PhongShader.h"
#include "../math.h"
#include <algorithm>
PIRenderer::Blinn_PhongShader::Blinn_PhongShader()
{
	m_ShadowMap = nullptr;
	m_UseShadow = false;
}

PIRenderer::Blinn_PhongShader::Blinn_PhongShader(float* shadowMap, int width, int height, bool useShadow = true)
	: m_ShadowMap(shadowMap), m_SMWidth(width), m_SMHeight(height), m_UseShadow(useShadow)
{
}

void PIRenderer::Blinn_PhongShader::FragmentShader(V2F* v)
{
	Vector3f objectColor;
	if (m_Texture)
		objectColor = (m_Texture->Sample(v->m_Texcoord.u, v->m_Texcoord.v));
	else
		objectColor = { 1, 1, 1 };
	float lightIntensity = m_DirectionLight.GetIntensity();

	//环境光
	float Ka = 0.2;
	float ambient = lightIntensity * Ka;

	//漫反射
	Vector3f normal = Vector3f::Normalize(v->m_Normal);

	if (m_NormalMap)
	{
		Vector3f tangent = Vector3f::Normalize(m_Tangent - (m_Tangent * normal) * normal);
		Vector3f bitangent = Vector3f::CrossProduct(tangent, normal);
		Matrix4 TBN = GetTBN(tangent, bitangent, normal);
		normal = (m_NormalMap->Sample(v->m_Texcoord.u, v->m_Texcoord.v));
		normal = (normal * 2.0f + -1.0f);
		normal.Normalize();
		normal = normal * TBN;
	}
	Vector3f lightDir = m_DirectionLight.GetDirection();
	float Kd = 0.7;
	float diff = std::max(normal * lightDir, 0.0f) * Kd;
	float diffuse = lightIntensity * diff;

	//镜面反射
	float Ks = 0.5;
	Vector3f half = Vector3f::Normalize((lightDir - m_EyePos));
	float spec = std::pow(std::max(normal * half, 0.0f), 32) * Ks;
	float specular = lightIntensity * spec;

	if (m_UseShadow && m_ShadowMap != nullptr)
	{
		float shadow = ShadowClaculation(v->m_WorldPos);
		v->m_Color = objectColor * (ambient + (1.0 - shadow) * diffuse + specular);
	}
	else
		v->m_Color = objectColor * (ambient + diffuse + specular);

	//tone mapping
	//v->m_Color = (v->m_Color) / (v->m_Color + 1.0);
}

void PIRenderer::Blinn_PhongShader::SetEyePos(const Vector3f& eyepos)
{
	m_EyePos = eyepos;
}

void PIRenderer::Blinn_PhongShader::SetShaowMap(float* shadowMap, int width, int height)
{
	m_ShadowMap = shadowMap;
	m_SMWidth = width;
	m_SMHeight = height;
}

void PIRenderer::Blinn_PhongShader::UseShadow(bool use)
{
	m_UseShadow = use;
}

void PIRenderer::Blinn_PhongShader::UsePCF(bool use)
{
	m_UsePCF = use;
	m_UsePCSS = (use == true ? false : m_UsePCSS);
	m_UseVSSM = (use == true ? false : m_UseVSSM);
}

void PIRenderer::Blinn_PhongShader::UsePCSS(bool use)
{
	m_UsePCSS = use;
	m_UsePCF = (use == true ? false : m_UsePCF);
	m_UseVSSM = (use == true ? false : m_UseVSSM);
}

void PIRenderer::Blinn_PhongShader::UseVSSM(bool use)
{
	m_UsePCF = (use == true ? false : m_UsePCF);
	m_UsePCSS = (use == true ? false : m_UsePCSS);
	m_UseVSSM = use;
}

void PIRenderer::Blinn_PhongShader::SetLightSpaceMatrix(const Matrix4& m)
{
	m_LightSpace = m;
}

void PIRenderer::Blinn_PhongShader::SetExSAT(std::vector<std::vector<double>>* ExSAT)
{
	m_ExSAT = ExSAT;
}

void PIRenderer::Blinn_PhongShader::SetExSquareSAT(std::vector<std::vector<double>>* ExSquareSAT)
{
	m_ExSquareSAT = ExSquareSAT;
}


float PIRenderer::Blinn_PhongShader::ShadowClaculation(const Vector3f& worldPos)
{
	Vector3f lightSpacePos = worldPos * m_LightSpace;
	float shadow = 0.0;

	lightSpacePos.x /= lightSpacePos.w;
	lightSpacePos.y /= lightSpacePos.w;
	lightSpacePos.z /= lightSpacePos.w;

	int x = (lightSpacePos.x + 1.0) * 0.5f * m_SMWidth;
	int y = (1.0 - lightSpacePos.y) * 0.5f * m_SMHeight;
	float closestDepth = m_ShadowMap[y * m_SMWidth + x];
	float currentDepth = lightSpacePos.z;

	//PCF
	if (m_UsePCF)
	{
		for(int i = -1; i <= 1; ++i)
			for (int j = -1; j <= 1; ++j)
			{
				int nx = x + i;
				int ny = y + j;
				closestDepth = m_ShadowMap[ny * m_SMWidth + nx];
				shadow += (currentDepth - 0.005 > closestDepth ? 1.0f : 0.0f);
			}

		return shadow / 9.0f;
	}
	//PCSS
	else if (m_UsePCSS)
	{
		int numSample = 20;
		float lightSize = 3;
		float zReceiver = currentDepth;

		poissonDiskSamples(Vector2(lightSpacePos.x, lightSpacePos.y), numSample);

		//1.遮挡深度
		float aveBlockDepth = findBlocker(x, y, zReceiver);
		//无遮挡
		if (aveBlockDepth == -1)
			return 0.0f;

		//2.半影范围
		float penumbra = lightSize * (zReceiver - aveBlockDepth) / aveBlockDepth;

		//3. pcf
		for (int i = 0; i < numSample; ++i)
		{
			int nx = x + poissonDisk[i].u * penumbra;
			int ny = y + poissonDisk[i].v * penumbra;

			closestDepth = m_ShadowMap[ny * m_SMWidth + nx];
			shadow += (currentDepth - 0.005 > closestDepth ? 1.0f : 0.0f);
		}

		return shadow / (float)numSample;
	}
	//vssm
	else if (m_UseVSSM)
	{
		//1. find blocker depth
		float lightSize = 50;
		float nearPlane = -0.9;
		//搜索半径不能用浮点数，不然在前缀和减法的时候会出现区域下标错误的情况
		int searchRadius = abs(lightSize * (currentDepth - nearPlane) / currentDepth) + 0.5;
		float ex, var;
		AreaSerach(&ex, &var, x, y, searchRadius);
		if (currentDepth <= ex) 
			return 0.0f;

		float zUnocc = currentDepth;
		float zAvg = ex;
		float pzUnocc = var / (var + (currentDepth - ex) * (currentDepth - ex));
		float pzOcc = 1.0 - pzUnocc;
		if (pzOcc == 0) 
			return 0.0;

		float zOcc = (zAvg - pzUnocc * zUnocc) / pzOcc;

		//2. compute penumbra
		float zReceiver = currentDepth;
		float aveBlockDepth = zOcc;
		int penumbra = abs(lightSize * (zReceiver - aveBlockDepth) / aveBlockDepth) + 0.5;


		//3. pcf
		AreaSerach(&ex, &var, x, y, penumbra);

		if (currentDepth <= ex) return 0.0f;

		shadow = 1.0 - var / (var + (currentDepth - ex) * (currentDepth - ex));
		return shadow;
	}

	//普通的SM
	shadow = (currentDepth - 0.005 > closestDepth ? 1.0f : 0.0f);
	return shadow;
}

float PIRenderer::Blinn_PhongShader::findBlocker(int x, int y, float zReceiver)
{
	int blockNums = 0;
	float blockDepth = 0;

	int numSample = 20;
	for (int i = 0; i < numSample; ++i)
	{
		int nx = x + poissonDisk[i].u * 2;
		int ny = y + poissonDisk[i].v * 2;

		float closestDepth = m_ShadowMap[ny * m_SMWidth + nx];
		if (zReceiver - 0.005 > closestDepth)
		{
			blockNums++;
			blockDepth += closestDepth;
		}
	}

	if (blockNums == 0)
		return -1;

	return blockDepth / (float)blockNums;

}

void PIRenderer::Blinn_PhongShader::AreaSerach(float* ex, float* var, float x, float y, int searchRadius)
{
	float ex_s;
	float size = (2 * searchRadius + 1) * (2 * searchRadius + 1);

	if (x - searchRadius - 1 < 0 || y - searchRadius - 1 < 0)
	{
		*ex = (*m_ExSAT)[y + searchRadius][x + searchRadius] /
			((y + searchRadius + 1) * (x + searchRadius + 1));

		ex_s = (*m_ExSquareSAT)[y + searchRadius][x + searchRadius] /
			((y + searchRadius + 1) * (x + searchRadius + 1));
	}
	else
	{
		*ex =
			((*m_ExSAT)[y + searchRadius][x + searchRadius] -
				(*m_ExSAT)[y - searchRadius - 1][x + searchRadius] -
				(*m_ExSAT)[y + searchRadius][x - searchRadius - 1] +
				(*m_ExSAT)[y - searchRadius - 1][x - searchRadius - 1]) / size;

		ex_s =
			((*m_ExSquareSAT)[y + searchRadius][x + searchRadius] -
				(*m_ExSquareSAT)[y - searchRadius - 1][x + searchRadius] -
				(*m_ExSquareSAT)[y + searchRadius][x - searchRadius - 1] +
				(*m_ExSquareSAT)[y - searchRadius - 1][x - searchRadius - 1]) / size;
	}

	*var = ex_s - (*ex) * (*ex);
}
