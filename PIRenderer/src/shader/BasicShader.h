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

		virtual V2F VertexShader(const Vertex& v) override;
		virtual void FragmentShader(V2F* v) override;

		virtual void SetTexture(Texture* texture) override;
		virtual void SetVPMatrix(Matrix4 vpMatrix) override;
		virtual void SetModelMatrix(Matrix4 Model) override;

		virtual void SetLight(DirectionLight dLight) override;
		virtual void SetEyePos(const Vector3f eyepos) override;

	protected:
		void Vertex_rhw_Init(V2F* v2f);

	public:
		DirectionLight m_DirectionLight;
		Vector3f n;

		Texture* m_Texture;
		Matrix4 m_VPMatrix;
		Matrix4 m_ModelMatrix;
		Matrix4 m_NormalMatrix;
	};
}

