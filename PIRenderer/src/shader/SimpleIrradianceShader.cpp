#include "SimpleIrradianceShader.h"
#include "../math.h"
namespace PIRenderer {
	void SimpleIrradianceShader::FragmentShader(V2F* v)
	{
		/*
		* 这段代码可以采样纹理
		float uu = v->m_Texcoord.u;
		float vv = v->m_Texcoord.v;
		m_IrradianceMap->Set(uu, vv, m_Texture->Sample(uu, vv));
		*/

		Vector3f normal = Vector3f::Normalize(v->m_WorldPos);//z-axis
		Vector3f up(0.0f, 1.0f, 0.0f);//y-axis
		Vector3f right = Vector3f::CrossProduct(up, normal); //x-axis
		up = Vector3f::CrossProduct(normal, right);

		Vector3f irrandiance(0.0f, 0.0f, 0.0f);

		float sampleDelta = 0.025;
		float nrSample = 0.0f;

		for (float phi = 0.0f; phi <= 2.0f * PI; phi += sampleDelta)
		{
			for (float theta = 0.0f; theta <= 0.5 * PI; theta += sampleDelta)
			{
				//球坐标-》笛卡尔
				Vector3f tangentSample = { cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta) };
				//世界坐标
				Vector3f sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

				irrandiance += m_CubeMap->Sample(sampleVec) * cos(theta) * sin(theta);
				nrSample++;
			}
		}
		irrandiance = PI * irrandiance / (float)nrSample;

		int face = m_CubeMap->GetFace(normal);
		Vector2 uv = m_CubeMap->GetUV(face, normal);
		m_IrradianceMap->Set(uv.u, uv.v, irrandiance);
	}
}