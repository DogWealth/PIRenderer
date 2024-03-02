#pragma once
#include <string>
#include "Vertex.h"
#include "Texture.h"
namespace PIRenderer {
	class V2F
	{
	public:
		V2F() = default;
		~V2F() = default;
		
		V2F(const Vector3f& worldPos, const Vector3f& screenPos, const Vector3f& normal, const Vector3f& color, const Vector2& texture);
		V2F(const V2F& v);

		static V2F* Interpolate(V2F* v, const V2F& v1, const V2F& v2, float t);

	public:
		Vector3f m_WorldPos;
		Vector3f m_ScreenPos;
		Vector3f m_Color;
		Vector3f m_Normal;
		Vector2 m_Texcoord;
		float m_rhw;

	};
	class Shader
	{
	public:
		virtual V2F VertexShader(const Vertex& a2v) = 0;
		virtual void FragmentShader(V2F* v) = 0;

		virtual void SetTexture(Texture* texture) = 0;
		virtual void SetVPMatrix(Matrix4 vpMatrix) = 0;
		virtual void SetModelMatrix(Matrix4 Model) = 0;

		static Shader* Create(const std::string& name);
	};
}

