#pragma once
#include "Matrix.h"
#include "Vertex.h"
#include <string>
#include <vector>
namespace PIRenderer {
	class Mesh
	{
	public:
		Mesh() {}
		Mesh(const std::string& path);

		void ReadObjFile(const std::string& path);

		const std::vector<Vertex>& GetVertexBuffer() const { return m_VertexBuffer; }

	private:
		std::vector<std::string> Split(const std::string& s, char delim);

	private:
		std::vector<Vertex> m_VertexBuffer;
		std::vector<Vector3f> m_PositionBuffer;
		std::vector<Vector3f> m_NormalBuffer;
		std::vector<Vector2> m_uvBuffer;
		std::vector<Vector3i> m_IndexBuffer;
	};
}

