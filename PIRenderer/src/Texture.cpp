#include "Texture.h"

namespace PIRenderer {
	Texture::Texture(const std::string& filepath)
	{
		LoadTexture(filepath);

		m_Width = m_TGAImage->get_width();
		m_Height = m_TGAImage->get_height();

	}

	Texture::~Texture()
	{
		delete m_TGAImage;
	}

	void Texture::LoadTexture(const std::string& filepath)
	{
		if (!m_TGAImage)
		{
			m_TGAImage = new TGAImage();
		}

		if (!m_TGAImage->read_tga_file(filepath.c_str()))
		{
			printf("Load texture failed!\n");
		}

		m_TGAImage->flip_vertically();
	}

	Vector3f Texture::Sample(float u, float v)
	{
		int tu = (int)(u * (m_Width - 1));
		int tv = (int)(v * (m_Height - 1));

		TGAColor m_TAGColor = m_TGAImage->get(tu, tv);

		return {m_TAGColor.bgra[2] / 255.0f, m_TAGColor.bgra[1] / 255.0f, m_TAGColor.bgra[0] / 255.0f };

	}

	CubeMap::CubeMap()
	{
		m_CubeMap.clear();
	}

	CubeMap::CubeMap(Texture* posx, Texture* posy, Texture* posz, Texture* negx, Texture* negy, Texture* negz)
	{
		m_CubeMap.clear();
		m_CubeMap.push_back(posx);
		m_CubeMap.push_back(posy);
		m_CubeMap.push_back(posz);
		m_CubeMap.push_back(negx);
		m_CubeMap.push_back(negy);
		m_CubeMap.push_back(negz);
	}

	void CubeMap::Clear()
	{
		m_CubeMap.clear();
	}

	void CubeMap::DelteAllTextures()
	{
		for (auto tex : m_CubeMap)
			delete tex;
	}

	void CubeMap::SetCubeMap(Texture* posx, Texture* posy, Texture* posz, Texture* negx, Texture* negy, Texture* negz)
	{
		m_CubeMap.clear();
		m_CubeMap.push_back(posx);
		m_CubeMap.push_back(posy);
		m_CubeMap.push_back(posz);
		m_CubeMap.push_back(negx);
		m_CubeMap.push_back(negy);
		m_CubeMap.push_back(negz);
	}

	void CubeMap::SetCubeMap(Texture* texture, int index)
	{
		if (index > 5 || index < 0) return;

		m_CubeMap[index] = texture;
	}

	Vector3f CubeMap::Sample(const Vector3f& dir)
	{
		//get face
		int face = GetFace(dir);

		//get uv
		Vector2 uv = GetUV(face, dir);

		//sample

		if (m_CubeMap.empty()) return Vector3f();

		return (m_CubeMap[face]->Sample(uv.u, uv.v));
	}

	int CubeMap::GetFace(const Vector3f& dir)
	{
		float maxValue = 0;
		int maxIndex = 0;
		float arr[3] = { dir.x, dir.y, dir.z };
		for (int i = 0; i < 3; i++)
		{
			if (abs(arr[i]) > maxValue)
			{
				maxIndex = i;
				maxValue = abs(arr[i]);
			}
		}

		return maxIndex + (arr[maxIndex] > 0 ? 0 : 3);
	}

	/*
	* ���ȶ���һ�����ߣ���ʼ��p0�ͷ���d����ô����������һ��p=p0 + td��
	* ƽ��Ķ�����ͨ��һ������n��һ��������ʵ�֣�����ƽ���ϵ����е�p��p��n = d��d��һ��������
	* ��ôҪ��������֮����ཻ�Ͳ����ˣ������ߺ�ƽ���ཻ������У�
	* (p0 + td)��n = d
	* p0��n + td��n = d
	* ���տɵã�
	* t=(d - p0��n��/(d��n)��
	* ������ߺ�ƽ�滥��ƽ�У���d��n = 0������֮��û�н��㡣d�ļ�����ͨ����ƽ���ϵ�һ����p��ȡp��n = d��ʵ�֡�
	* 
	* ��t������
	* ������һ����λ1��������������������ĳ���
	* uv�ķ�Χ��0 - 1�� ����������-1 - 1 Ҫ����ת��
	*/
	Vector2 CubeMap::GetUV(int face, const Vector3f& dir)
	{
		float u, v, t;
		switch (face)
		{
		case 0://posx : u = -z, v = +y
			t = 1.0f / dir.x;
			u = (-dir.z * t + 1.0f) * 0.5f;
			v = (dir.y * t + 1.0f) * 0.5f;
			break;
		case 1://posy : u = +x, v = -z
			t = 1.0f / dir.y;
			u = (dir.x * t + 1.0f) * 0.5f;
			v = (-dir.z * t + 1.0f) * 0.5f;
			break;
		case 2://posz : u = +x, v = +y
			t = 1.0f / dir.z;
			u = (dir.x * t + 1.0f) * 0.5f;
			v = (dir.y * t + 1.0f) * 0.5f;
			break;
		case 3://negx : u = +z, v = +y
			t = 1.0f / -dir.x;
			u = (dir.z * t + 1.0f) * 0.5f;
			v = (dir.y * t + 1.0f) * 0.5f;
			break;
		case 4://negy : u = +x, v = +z
			t = 1.0f / -dir.y;
			u = (dir.x * t + 1.0f) * 0.5f;
			v = (dir.z * t + 1.0f) * 0.5f;
			break;
		case 5://posz : u = -x, v = +y
			t = 1.0f / -dir.z;
			u = (-dir.x * t + 1.0f) * 0.5f;
			v = (dir.y *t + 1.0f) * 0.5f;
			break;
		default:
			break;
		}
		return Vector2(u, v);
	}

}