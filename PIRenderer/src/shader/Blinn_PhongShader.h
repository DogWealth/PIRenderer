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
		void UseVSSM(bool use);
		void SetLightSpaceMatrix(const Matrix4& m);
		void SetExSAT(std::vector<std::vector<double>>* ExSAT);
		void SetExSquareSAT(std::vector<std::vector<double>>* ExSquareSAT);

	private:
		float ShadowClaculation(const Vector3f& worldPos);
		float findBlocker(int x, int y, float zReceiver);
		void AreaSerach(float* ex, float* var, float x, float y, int searchRadius);

	private:
		Vector3f m_EyePos;
		Matrix4 m_LightSpace;
		float* m_ShadowMap;
		int m_SMWidth;
		int m_SMHeight;
		bool m_UseShadow = false;
		bool m_UsePCF = false;
		bool m_UsePCSS = false;
		bool m_UseVSSM = false;
		std::vector<std::vector<double>>* m_ExSAT;
		std::vector<std::vector<double>>* m_ExSquareSAT;
	};
}

