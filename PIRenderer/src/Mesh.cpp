#include "Mesh.h"
#include <sstream>
#include <fstream>
#include <assert.h>

namespace PIRenderer {
	Mesh::Mesh(const std::string& path)
	{
		ReadObjFile(path);
	}

	void Mesh::ReadObjFile(const std::string& path)
	{
		std::ifstream in;
		in.open(path.c_str(), std::ifstream::in);
		if (in.fail())
			assert(0);

		std::string line;
		while (std::getline(in, line))
		{
			std::vector<std::string> elements = Split(line, ' ');
			if (elements.size() == 0)	continue;

			if (elements[0] == "v")
			{
				m_PositionBuffer.emplace_back(
					atof(elements[1].c_str()), 
					atof(elements[2].c_str()),
					atof(elements[3].c_str())
					);
			}

			if (elements[0] == "vn")
			{
				m_NormalBuffer.emplace_back(
					atof(elements[1].c_str()),
					atof(elements[2].c_str()),
					atof(elements[3].c_str())
				);
			}

			if (elements[0] == "vt")
			{
				m_uvBuffer.emplace_back(
					atof(elements[1].c_str()),
					atof(elements[2].c_str())
				);
			}

			if (elements[0] == "f")
			{
				//先把f的数据取出来，每一组x/x/x 作为一个indexbuffer的成员，代表一个vertex的属性的索引
				for (int i = 1; i <= 3; i++)
				{
					std::vector<std::string> idx = Split(elements[i], '/');
					m_IndexBuffer.emplace_back(
						atoi(idx[0].c_str()) - 1,
						atoi(idx[1].c_str()) - 1,
						atoi(idx[2].c_str()) - 1
						);

					//构造vertexBuffer
					Vector3i index = m_IndexBuffer.back();
					Vector3f position = m_PositionBuffer[index.x];
					Vector2 uv = m_uvBuffer[index.y];
					Vector3f normal = m_NormalBuffer[index.z];

					m_VertexBuffer.emplace_back(position, normal, uv);
				}
			}
		}

	}

	std::vector<std::string> Mesh::Split(const std::string& s, char delim)
	{
		std::istringstream iss(s);
		std::vector<std::string> res;
		std::string buffer;

		while (getline(iss, buffer, delim))
		{
			res.push_back(buffer);
		}

		return res;
	}

	

}
