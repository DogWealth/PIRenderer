#pragma once
#include <stdint.h>
#include "PIMath.h"
namespace PIRenderer {
	class Canvas
	{
	public:
		Canvas(uint32_t* framBuffer, int width, int height);
		~Canvas();

		void SetPixel(int x, int y, int z, uint32_t color);
		void SetPixel(int x, int y, int z, Vector3 color);

		void DrawTriangle(Vector3 v1, Vector3 v2, Vector3 v3, const Vector3& color);//scanline

		void SetRotationMatrix(float yaw, float pitch, float roll);

		void Clear();

	private:
		uint32_t* m_FramBuffer;

		Matrix4 m_RotationMatrix;

		int m_Width;
		int m_Height;
	};
}