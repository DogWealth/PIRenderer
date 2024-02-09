#include "Renderer.h"
#include <string>
#include <algorithm>
#include <time.h>
namespace PIRenderer {
	Renderer::Renderer(uint32_t* framBuffer, int width, int height)
		: m_FramBuffer(framBuffer), m_Width(width), m_Height(height)
	{
		m_DepthBuffer = new float[width * height];

		m_Shader = nullptr;
	}

	Renderer::~Renderer()
	{
		delete[] m_DepthBuffer;
	}

	void Renderer::SetPixel(int x, int y, float z, uint32_t color)
	{
		if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
			return;

		int index = y * m_Width + x;
		float depth = m_DepthBuffer[index];
		if (depth < z)
		{
			return;
		}
		m_DepthBuffer[index] = z;
		m_FramBuffer[index] = color;
	}

	void Renderer::SetPixel(int x, int y, float z, const Vector3f& color)
	{
		uint32_t A = 0.0f;
		uint32_t R = (uint32_t)(color.x * 255.f);
		uint32_t G = (uint32_t)(color.y * 255.f);
		uint32_t B = (uint32_t)(color.z * 255.f);

		uint32_t Color = A | (R << 16) | (G << 8) | B;

		SetPixel(x, y, z, Color);
	}

	void Renderer::DrawTriangle(Vector3f v1, Vector3f v2, Vector3f v3, const Vector3f& color)
	{
		if (v1.y > v2.y) std::swap(v1, v2);
		if (v1.y > v3.y) std::swap(v1, v3);
		if (v2.y > v3.y) std::swap(v2, v3);

		float totalH = v3.y - v1.y;

		//上三角形
		for (int y = v1.y + 1; y <= v2.y; y++)
		{
			float halfH = v2.y - v1.y + 1e-6;

			float step_12 = (float)(y - v1.y) / halfH;
			float step_13 = (float)(y - v1.y) / totalH;

			Vector3f v_12 = v1 + (v2 - v1) * step_12;
			Vector3f v_13 = v1 + (v3 - v1) * step_13;

			if (v_12.x > v_13.x) std::swap(v_12, v_13);

			for (int x = v_12.x; x < v_13.x; x++)
			{
				float t = (float)(x - v_12.x) / (v_13.x - v_12.x);
				Vector3f v = Vector3f::Interpolate(v_12, v_13, t);
				SetPixel(v.x, v.y, v.z, color);
			}
		}

		//下三角形
		for (int y = v2.y + 1; y <= v3.y; y++)
		{
			float halfH = v3.y - v2.y + 1e-6;

			float step_23 = (float)(y - v2.y) / halfH;
			float step_13 = (float)(y - v1.y) / totalH;

			Vector3f v_23 = v2 + (v3 - v2) * step_23;
			Vector3f v_13 = v1 + (v3 - v1) * step_13;

			if (v_23.x > v_13.x) std::swap(v_23, v_13);

			for (int x = v_23.x; x < v_13.x; x++)
			{
				float t = (float)(x - v_23.x) / (v_13.x - v_23.x);
				Vector3f v = Vector3f::Interpolate(v_23, v_13, t);
				SetPixel(v.x, v.y, v.z, color);
			}
		}
	}

	void Renderer::DrawLine(Vertex* v1, Vertex* v2)
	{
		int x1 = (int)v1->m_Position.x;
		int y1 = (int)v1->m_Position.y;
		int x2 = (int)v2->m_Position.x;
		int y2 = (int)v2->m_Position.y;

		// delta x and delta y
		int dx = x2 - x1;
		int dy = y2 - y1;

		if (abs(dx) > abs(dy)) {
			int sign = x2 - x1 > 0 ? 1 : -1;
			float ratio = 0;
			if (dx != 0) {
				ratio = (float)dy / dx;
			}

			for (int x = x1; x != x2; x += sign) {
				int y = y1 + (x - x1) * ratio;
				
				SetPixel(x, y, 0, { 1, 1, 1 });
			}
		}
		else {
			int sign = y2 - y1 > 0 ? 1 : -1;
			float ratio = 0;
			if (dy != 0) {
				ratio = (float)dx / dy;
			}

			for (int y = y1; y != y2; y += sign) {
				int x = x1 + (y - y1) * ratio;
				SetPixel(x, y, 0, { 1, 1, 1 });
			}
		}
	}

	void Renderer::DrawScanline(Vertex* v, Vertex* v1, Vertex* v2)
	{
		if (v1->m_Position.x > v2->m_Position.x)
			std::swap(v1, v2);

		int x1 = v1->m_Position.x;
		int x2 = v2->m_Position.x;

		for (int x = x1; x < x2; x++)
		{
			float t = (float)(x - x1) / (x2 - x1);
			Vertex::Interpolate(v, *v1, *v2, t);

			//Early-Z
			int index = (int)v->m_Position.y * m_Width + (int)v->m_Position.x;

			//cliping
			if (index < 0 || index >= m_Width * m_Height)
			{
				continue;
			}

			float depth = m_DepthBuffer[index];
			if (depth < v->m_Position.z)
			{
				continue;
			}
			m_DepthBuffer[index] = v->m_Position.z;

			m_Shader->FragmentShader(v);

			SetPixel(v->m_Position.x, v->m_Position.y, v->m_Position.z, v->m_Color);
		}
	}

	void Renderer::DrawTriangleLine(Vertex* v1, Vertex* v2, Vertex* v3)
	{
		DrawLine(v1, v2);
		DrawLine(v1, v3);
		DrawLine(v2, v3);
	}

	void Renderer::DrawTriangle(Vertex* v1, Vertex* v2, Vertex* v3)
	{
		if (v1->m_Position.y > v2->m_Position.y) std::swap(v1, v2);
		if (v1->m_Position.y > v3->m_Position.y) std::swap(v1, v3);
		if (v2->m_Position.y > v3->m_Position.y) std::swap(v2, v3);

		Vector3f p1 = v1->m_Position;
		Vector3f p2 = v2->m_Position;
		Vector3f p3 = v3->m_Position;

		float totalH = p3.y - p1.y;

		//上三角形
		Vertex v_12;
		Vertex v_13;
		Vertex v_23;
		Vertex v;


		for (int y = p1.y + 1; y <= p2.y; y++)
		{
			float halfH = p2.y - p1.y + 1e-8;

			float t_12 = (float)(y - p1.y) / halfH;
			float t_13 = (float)(y - p1.y) / totalH;

			Vertex::Interpolate(&v_12, *v1, *v2, t_12);
			Vertex::Interpolate(&v_13, *v1, *v3, t_13);

			if (v_12.m_Position.x > v_13.m_Position.x) std::swap(v_12, v_13);

			DrawScanline(&v, &v_12, &v_13);
		}

		//下三角形
		for (int y = p2.y + 1; y <= p3.y; y++)
		{
			float halfH = p3.y - p2.y + 1e-8;
			float t_23 = (float)(y - p2.y) / halfH;
			float t_13 = (float)(y - p1.y) / totalH;


			Vertex::Interpolate(&v_23, *v2, *v3, t_23);
			Vertex::Interpolate(&v_13, *v1, *v3, t_13);


			if (v_23.m_Position.x > v_13.m_Position.x) std::swap(v_23, v_13);

			DrawScanline(&v, &v_23, &v_13);
		}

	}

	void Renderer::DrawMeshLine(Mesh* mesh)
	{
		const std::vector<Vertex>& vertexs = mesh->GetVertexBuffer();

		if (vertexs.size() == 0) return;

		for (int i = 0; i < vertexs.size(); i += 3)
		{
			Vertex v1 = vertexs[i];
			Vertex v2 = vertexs[i + 1];
			Vertex v3 = vertexs[i + 2];

			ViewPort(&v1.m_Position);
			ViewPort(&v2.m_Position);
			ViewPort(&v3.m_Position);

			DrawTriangleLine(&v1, &v2, &v3);
		}

	}

	void Renderer::DrawMesh(Mesh* mesh)
	{
		const std::vector<Vertex>& vertexs = mesh->GetVertexBuffer();
		
		if (vertexs.size() == 0) return;

		double time1 = clock();
		for (int i = 0; i < vertexs.size(); i += 3)
		{
			Vertex v1 = vertexs[i];
			Vertex v2 = vertexs[i + 1];
			Vertex v3 = vertexs[i + 2];

			m_Shader->VertexShader(&v1, &v2, &v3);

			ViewPort(&v1.m_Position);
			ViewPort(&v2.m_Position);
			ViewPort(&v3.m_Position);

			DrawTriangle(&v1, &v2, &v3);
		}
		double time2 = clock();
		//printf("FPS: %lf\n", 1000 / (time2 - time1));
	}

	void Renderer::DrawMeshLines()
	{
		for (auto mesh : m_Meshs)
		{
			DrawMeshLine(mesh);
		}
	}

	void Renderer::DrawMeshs()
	{
		for (auto mesh : m_Meshs)
		{
			DrawMesh(mesh);
		}
	}

	void Renderer::BindShader(Shader* shader)
	{
		m_Shader = shader;
	}
	

	void Renderer::Clear()
	{
		memset(m_FramBuffer, 0, sizeof(uint32_t) * m_Width * m_Height);
		std::fill(m_DepthBuffer, m_DepthBuffer + m_Width * m_Height, FLT_MAX);

	}

	void Renderer::AddMesh(Mesh* mesh)
	{
		m_Meshs.push_back(mesh);
	}

	void Renderer::ViewPort(Vector3f* pos)
	{
		pos->x = (pos->x + 1.0f) * 0.5f * m_Width;
		pos->y = (1.0f - pos->y) * 0.5f * m_Height; //坐标反转了
	}
}