#pragma once
#include <stdint.h>
#include "PIMath.h"
#include "Vertex.h"
namespace PIRenderer {
	class Renderer
	{
	public:
		Renderer(uint32_t* framBuffer, int width, int height);
		~Renderer();

		void SetPixel(int x, int y, uint32_t color);
		void SetPixel(int x, int y, Vector3 color);

		void DrawTriangle(Vector3 v1, Vector3 v2, Vector3 v3, const Vector3& color);//scanline

		void DrawScanline(Vertex v1, Vertex v2);
		void DrawTriangle(Vertex v1, Vertex v2, Vertex v3);//scanline

		void SetRotationMatrix(float yaw, float pitch, float roll);

		void Clear();

	private:
		uint32_t* m_FramBuffer;

		Matrix4 m_RotationMatrix;

		int m_Width;
		int m_Height;
	};
}