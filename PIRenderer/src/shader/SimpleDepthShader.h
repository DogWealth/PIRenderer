#pragma once
#include "BasicShader.h"
namespace PIRenderer {
	class SimpleDepthShader : public BasicShader
	{
	public:
		virtual void FragmentShader(V2F* v) override;
	};
}

