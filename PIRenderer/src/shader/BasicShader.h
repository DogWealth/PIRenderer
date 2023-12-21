#pragma once
#include "../Shader.h"
#include "../Light.h"
#include "../Texture.h"
#include "../Matrix.h"
namespace PIRenderer {
	class BasicShader : public Shader
	{
	public:
		BasicShader();
		virtual ~BasicShader();

		virtual void VertexShader(Vertex* v1, Vertex* v2, Vertex* v3) override;
		virtual void FragmentShader(Vertex* v) override;

		virtual void SetTexture(Texture* texture) override;
		virtual void SetVPMatrix(Matrix4 vpMatrix) override;

	public:
		DirectionLight m_DirectionLight;
		Vector3f n;

		Texture* m_Texture;
		Matrix4 m_VPMatrix;
	};
}

