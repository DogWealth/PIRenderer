#pragma once
#include "BasicShader.h"
namespace PIRenderer {
	class SkyBoxShader : public BasicShader
	{
	public:
		virtual void FragmentShader(V2F* v) override;

		void SetCubeMap(CubeMap* cubemap);
	private:
		CubeMap* m_CubeMap;
	};

}
