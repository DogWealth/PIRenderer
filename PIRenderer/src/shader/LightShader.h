#pragma once
#include "BasicShader.h"
namespace PIRenderer {
	class LightShader : public BasicShader
	{
	public:
		virtual void FragmentShader(V2F* v) override;
	};
}

