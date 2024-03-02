#pragma once
#include <stdint.h>
#include "Matrix.h"
#include "Vertex.h"
#include "Vector.h"
#include "Mesh.h"
#include "Light.h"
#include "Shader.h"
#include "Camera.h"
#include "CameraController.h"
namespace PIRenderer {
	class Renderer
	{
	public:
		Renderer(uint32_t* framBuffer, int width, int height);
		~Renderer();

		void SetPixel(int x, int y, float z, uint32_t color);
		void SetPixel(int x, int y, float z, const Vector3f& color);


		void DrawTriangle(Vector3f v1, Vector3f v2, Vector3f v3, const Vector3f& color);//scanline

		void DrawLine(Vertex* v1, Vertex* v2);
		void DrawScanline(V2F* v, V2F* v1, V2F* v2);

		void DrawTriangleLine(Vertex* v1, Vertex* v2, Vertex* v3);
		void DrawTriangle(V2F* v1, V2F* v2, V2F* v3);//scanline

		void DrawMeshLine(Mesh* mesh);
		void DrawMesh(Mesh* mesh);

		void DrawMeshLines();
		void DrawMeshs();

		void BindShader(Shader* shader);


		void Clear();

		void AddMesh(Mesh* mesh);

	private:
		void ViewPort(Vector3f* pos);
		bool FaceCulling(const Vector3f& v1, const Vector3f v2, const Vector3f v3);
		bool ViewCulling(const Vector3f& v1, const Vector3f v2, const Vector3f v3);
		void PerspectiveDivision(Vector3f* v);

	private:
		uint32_t* m_FramBuffer;
		float* m_DepthBuffer;

		Shader* m_Shader;

		std::vector<Mesh*> m_Meshs;

		DirectionLight m_DirectionLight;

		int m_Width;
		int m_Height;
	};
}