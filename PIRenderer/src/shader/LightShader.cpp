#include "LightShader.h"

void PIRenderer::LightShader::FragmentShader(V2F* v)
{
	v->m_Color = { 1, 1, 1 };
}
