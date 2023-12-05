#include "Canvas.h"
#include <string>
#include <algorithm>

namespace PIRenderer {
	Canvas::Canvas(uint32_t* framBuffer, int width, int height)
		: m_FramBuffer(framBuffer), m_Width(width), m_Height(height)
	{
	}

	Canvas::~Canvas()
	{
	}

	void Canvas::SetPixel(int x, int y, int z, uint32_t color)
	{
		int index = x * m_Width + y;
		m_FramBuffer[index] = color;
	}

	void Canvas::SetPixel(int x, int y, int z, Vector3 color)
	{
		uint32_t A = 0.0f;
		uint32_t R = (uint32_t)color.x * 255.f;
		uint32_t G = (uint32_t)color.y * 255.f;
		uint32_t B = (uint32_t)color.z * 255.f;

		uint32_t Color = A | (R << 16) | (G << 8) | B;

		int index = y * m_Width + x;
		m_FramBuffer[index] = Color;
	}

	void Canvas::DrawTriangle(Vector3 v1, Vector3 v2, Vector3 v3, const Vector3& color)
	{
		if (v1.y > v2.y) std::swap(v1, v2);
		if (v1.y > v3.y) std::swap(v1, v3);
		if (v2.y > v3.y) std::swap(v2, v3);

		int totalH = v3.y - v1.y;

		//上三角形
		for (int y = v1.y; y <= v2.y; y++)
		{
			double halfH = v2.y - v1.y + 1e-6;

			float step_12 = (float)(y - v1.y) / halfH;
			float step_13 = (float)(y - v1.y) / totalH;

			Vector3 v_12 = v1 + (v2 - v1) * step_12;
			Vector3 v_13 = v1 + (v3 - v1) * step_13;

			if (v_12.x > v_13.x) std::swap(v_12, v_13);

			for (int x = v_12.x; x < v_13.x; x++)
			{
				SetPixel(x, y, 0, color);
			}
		}

		//下三角形
		for (int y = v2.y; y <= v3.y; y++)
		{
			double halfH = v3.y - v2.y + 1e-6;

			float step_23 = (float)(y - v2.y) / halfH;
			float step_13 = (float)(y - v1.y) / totalH;

			Vector3 v_23 = v2 + (v3 - v2) * step_23;
			Vector3 v_13 = v1 + (v3 - v1) * step_13;

			if (v_23.x > v_13.x) std::swap(v_23, v_13);

			for (int x = v_23.x; x < v_13.x; x++)
			{
				SetPixel(x, y, 0, color);
			}
		}
	}

	void Canvas::Clear()
	{
		memset(m_FramBuffer, 0, sizeof(uint32_t) * m_Width * m_Height);
	}
}