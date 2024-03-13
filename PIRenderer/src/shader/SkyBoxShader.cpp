#include "SkyBoxShader.h"

namespace PIRenderer {
	void SkyBoxShader::FragmentShader(V2F* v)
	{
		v->m_Color = m_CubeMap->Sample(v->m_WorldPos);
	}

	void SkyBoxShader::SetCubeMap(CubeMap* cubemap)
	{
		m_CubeMap = cubemap;
	}
}
