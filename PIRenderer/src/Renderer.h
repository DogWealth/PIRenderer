#pragma once
#include <stdint.h>
#include "Matrix.h"
#include "Vertex.h"
#include "Vector.h"
#include "Mesh.h"
#include "Light.h"
#include "Shader.h"
namespace PIRenderer {
	class Renderer
	{
	public:
		Renderer(uint32_t* framBuffer, int width, int height);
		~Renderer();

		void SetPixel(int x, int y, float z, uint32_t color);
		void SetPixel(int x, int y, float z, Vector3f color);


		void DrawTriangle(Vector3f v1, Vector3f v2, Vector3f v3, const Vector3f& color);//scanline

		void DrawScanline(Vertex v1, Vertex v2);
		void DrawTriangle(Vertex v1, Vertex v2, Vertex v3);//scanline
		void DrawMesh(const Mesh& mesh);

		void SetRotationMatrix(float yaw, float pitch, float roll);

		void Clear();

	private:
		void ProjectToScreenSpace(Vector3f* pos);

	private:
		uint32_t* m_FramBuffer;
		float* m_DepthBuffer;

		Shader* m_Shader;

		DirectionLight m_DirectionLight;

		Matrix4 m_RotationMatrix;

		int m_Width;
		int m_Height;
	};
}