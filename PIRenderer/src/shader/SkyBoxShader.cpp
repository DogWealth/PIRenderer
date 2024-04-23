#include "SkyBoxShader.h"

namespace PIRenderer {
	void SkyBoxShader::FragmentShader(V2F* v)
	{
		v->m_Color = m_CubeMap->Sample(v->m_WorldPos);
		//v->m_Color = m_CubeMap->TrilinearSample(v->m_WorldPos, 1);
		
	}

	void SkyBoxShader::SetCubeMap(CubeMap* cubemap)
	{
		m_CubeMap = cubemap;
	}
}
