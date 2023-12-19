#include "Shader.h"
#include "shader/BasicShader.h"
namespace PIRenderer {
	Shader* Shader::Create(const std::string& name)
	{
		if (name == "BasicShader")
			return new BasicShader();

		return nullptr;
	}
}