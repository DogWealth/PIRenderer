#pragma once
#include "BasicShader.h"
namespace PIRenderer {
	class SimpleIrradianceShader : public BasicShader
	{
	public:
		virtual void FragmentShader(V2F* v) override;
		void SetIrradianceMap(Texture* texture) { m_IrradianceMap = texture; }
		void SetCubeMap(CubeMap* cubemap) { m_CubeMap = cubemap; }

	protected:
		CubeMap* m_CubeMap;

	private:
		Texture* m_IrradianceMap;
	};
}

