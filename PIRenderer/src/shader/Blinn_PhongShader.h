#pragma once
#include "BasicShader.h"
namespace PIRenderer {
	class Blinn_PhongShader : public BasicShader
	{
	public:
		Blinn_PhongShader() = default;
		virtual void FragmentShader(V2F* v) override;
		void SetEyePos(const Vector3f& eyepos);

	private:
		Vector3f m_EyePos;
	};
}

