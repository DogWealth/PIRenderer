#pragma once
#include <string>
#include "Vertex.h"
namespace PIRenderer {
	class Shader
	{
	public:
		virtual void VertexShader(Vertex* v1, Vertex* v2, Vertex* v3) = 0;
		virtual void FragmentShader(Vertex* v) = 0;

		static Shader* Create(const std::string& name);
	};
}

