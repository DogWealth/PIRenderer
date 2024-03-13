#pragma once
#include <stdint.h>
#include <SDL.h>
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
		Renderer(uint32_t* framBuffer, float* depthBuffer, int width, int height);
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

		void SetDepthBuffer(float* depthBuffer);
		void SetViewPort(int width, int height);


	private:
		void ViewPort(Vector3f* pos);
		bool FaceCulling(const Vector3f& v1, const Vector3f v2, const Vector3f v3);//背面剔除
		bool ViewCulling(const Vector3f& v1, const Vector3f v2, const Vector3f v3);
		void PerspectiveDivision(V2F* v);
		//视锥剔除和齐次裁剪
		bool InsidePlane(const Vector3f& plane, const Vector3f& pos);
		bool AllInsidePlane(const V2F& v1, const V2F& v2, const V2F& v3);
		V2F Intersect(const V2F& v1, const V2F& v2, const Vector3f& plane);
		std::vector<V2F> SutherlandHodgeman(const V2F& v1, const V2F& v2, const V2F& v3);//需要优化


	private:
		uint32_t* m_FramBuffer;
		float* m_DepthBuffer;
		Shader* m_Shader;
		std::vector<Mesh*> m_Meshs;
		DirectionLight m_DirectionLight;
		int m_Width;
		int m_Height;
		const std::vector<Vector3f> Viewplanes = {
			//near plane
			{0, 0, 1, 1},
			//far plane
			{0, 0, -1, 1},
			//left plane
			{1, 0, 0, 1},
			//right plane
			{-1, 0, 0, 1},
			//top plane
			{0, -1, 0, 1},
			//bottom plane
			{0, 1, 0, 1}
		};

	public:
		SDL_Window* m_Window;//调试用
	};
}