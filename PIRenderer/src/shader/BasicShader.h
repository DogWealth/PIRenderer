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

		void SetTexture(Texture* texture) override;
		void SetVPMatrix(Matrix4 vpMatrix) override;
		void SetModelMatrix(Matrix4 Model) override;

		void SetLight(DirectionLight dLight) override;
		void SetEyePos(const Vector3f& eyepos) override;

		virtual void SetTBN(const Matrix4& tbn)  override { m_TBN = tbn; }
		virtual void SetTangent(const Vector3f& t) override { m_Tangent = t; }

		virtual void SetNormalMap(Texture* normalMap) override { m_NormalMap = normalMap; }

	protected:
		void Vertex_rhw_Init(V2F* v2f);

	public:
		DirectionLight m_DirectionLight;
		Vector3f n;
		Vector3f m_eyePos;
		Vector3f m_Tangent;

		Texture* m_Texture;
		Texture* m_NormalMap;
		Matrix4 m_VPMatrix;
		Matrix4 m_ModelMatrix;
		Matrix4 m_NormalMatrix;

		Matrix4 m_TBN;
	};
}

