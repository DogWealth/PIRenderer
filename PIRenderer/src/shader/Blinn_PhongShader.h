#pragma once
#include "BasicShader.h"
namespace PIRenderer {
	class Blinn_PhongShader : public BasicShader
	{
	public:
		Blinn_PhongShader();
		Blinn_PhongShader(float* shadowMap, int width, int height, bool useShadow);
		virtual void FragmentShader(V2F* v) override;
		void SetEyePos(const Vector3f& eyepos);
		void SetShaowMap(float* shadowMap, int width, int height);
		void UseShadow(bool use);
		void UsePCF(bool use);
		void UsePCSS(bool use);
		void SetLightSpaceMatrix(const Matrix4& m);

	private:
		float ShadowClaculation(const Vector3f& worldPos);
		float findBlocker(int x, int y, float zReceiver);

	private:
		Vector3f m_EyePos;
		Matrix4 m_LightSpace;
		float* m_ShadowMap;
		int m_SMWidth;
		int m_SMHeight;
		bool m_UseShadow = false;
		bool m_UsePCF = false;
		bool m_UsePCSS = false;
	};
}

