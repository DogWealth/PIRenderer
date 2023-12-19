#pragma once
#include "../Shader.h"
#include "../Light.h"
#include "../Texture.h"
namespace PIRenderer {
	class BasicShader : public Shader
	{
	public:
		BasicShader();
		virtual ~BasicShader();

		virtual void VertexShader(Vertex* v1, Vertex* v2, Vertex* v3) override;
		virtual void FragmentShader(Vertex* v) override;

	public:
		DirectionLight m_DirectionLight;
		Vector3f n;

		Texture* m_Texture;
	};
}

