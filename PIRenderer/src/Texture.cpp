#include "Texture.h"
#include <format>
namespace PIRenderer {
	Texture::Texture(int width, int height, int bytespp)
	{
		m_TGAImage = TGAImage(width, height, bytespp);
		m_Width = m_TGAImage.get_width();
		m_Height = m_TGAImage.get_height();
	}

	Texture::Texture(const std::string& filepath)
	{
		LoadTexture(filepath);

		m_Width = m_TGAImage.get_width();
		m_Height = m_TGAImage.get_height();

	}

	Texture::~Texture()
	{
		m_TGAImage.clear();
	}

	void Texture::LoadTexture(const std::string& filepath)
	{
		if (!m_TGAImage.read_tga_file(filepath.c_str()))
		{
			printf("Load texture failed!\n");
		}

		m_TGAImage.flip_vertically();
	}

	Vector3f Texture::Sample(float u, float v)
	{
		int tu = (int)(u * (m_Width - 1));
		int tv = (int)(v * (m_Height - 1));

		TGAColor m_TAGColor;
		Vector3f color = { 0, 0, 0 };
		if (m_SampleMode == Nearest)
		{
			m_TAGColor = m_TGAImage.get(tu, tv);
			color =
			{m_TAGColor.bgra[2]/255.0f, m_TAGColor.bgra[1]/255.0f, m_TAGColor.bgra[0]/255.0f };
		}
		else if (m_SampleMode == Bilinear)
		{
			//只考虑正方形texture
			color = BilinearSample(u, v);
		}

		return color;

	}

	Vector3f Texture::Sample(const Vector2& texcoord)
	{
		float u = texcoord.u;
		float v = texcoord.v;
		int du = texcoord.du * (m_Width - 1);
		int dv = texcoord.dv * (m_Height - 1);

		float lod = 0;
		float dx2 = du * du + dv * dv;
		if(dx2 > 1) lod = 0.5 * log2(du * du + dv * dv);

		//可视化
		/*if (lod == 0) return { 0, 0, 0 };
		if (lod > 0 && lod <= 1) return { 0, 0, 1 };
		if (lod > 1 && lod <= 2) return { 0, 1, 0 };
		if (lod > 2 && lod <= 3) return { 0, 1, 1 };
		if (lod > 3 && lod <= 4) return { 1, 0, 0 };
		if (lod > 5 && lod <= 6) return { 1, 0, 1 };
		if (lod > 6 && lod <= 7) return { 1, 1, 0 };
		if (lod > 7 && lod <= 8) return { 1, 1, 1 };*/

		return SampleLod(u, v, lod);
	}

	Vector3f Texture::SampleLod(float u, float v, float lod)
	{
		lod = std::min(lod, (float)m_MipMaps.size());

		if (lod == 0)//0级mipmap就是本身
		{
			return Sample(u, v);
		}

		int d = (int)lod;
		if (lod - d == 0)
		{
			return m_MipMaps[d - 1].Sample(u, v);
		}

		//trilinear
		int d1 = d + 1;
		float t = (lod - (float)d);
		Texture& mipmapd = (d == 0 ? *this : m_MipMaps[d - 1]);
		Vector3f colord = mipmapd.BilinearSample(u, v);
		Vector3f colord1 = m_MipMaps[d1 - 1].BilinearSample(u, v);

		return Vector3f::Interpolate(colord, colord1, t);
	}

	void Texture::Set(float u, float v, const Vector3f& c)
	{
		TGAColor color = TGAColor(c.x * 255.f, c.y * 255.f, c.z * 255.f, 0);
		int tu = (int)(u * (m_Width - 0.5));
		int tv = (int)(v * (m_Height - 0.5));

		

		m_TGAImage.set(tu, tv, color);
	}

	bool Texture::Save(const std::string& filename)
	{
		m_TGAImage.flip_vertically();
		if (!(m_TGAImage.write_tga_file(filename.c_str())))
			return false;
		return true;
	}

	void Texture::GenerateMipMap(int maxLevel)
	{
		if (m_Height != m_Width)
		{
			printf("只支持正方形纹理！\n");
			return;
		}
		int n = m_Width;
		int level = log2(n);
		level = std::min(level, maxLevel);
		double texelSize = 1.0f / (double)(n - 1); //uv空间的大小

		//from 1 to level
		for (int l = 1; l < level; l++)
		{
			int nTextureSize = n / pow(2, l);
			double nTexelSize = 1.0f / (double)(nTextureSize - 1.0f);

			Texture tex(nTextureSize, nTextureSize);

			double nu = 0, nv = 0;
			for (double i = 0; i <= 1; i += (texelSize * pow(2, l)))
			{
				for (double j = 0; j <= 1; j += (texelSize * pow(2, l)))
				{
					Vector3f color = { 0, 0, 0 };
					int nrcolor = 0;
					for (double m = i; m < i + (texelSize * pow(2, l)); m += texelSize)
					{
						for (double n = j; n < j + (texelSize * pow(2, l)); n += texelSize)
						{
							color += this->Sample(m, n);
							nrcolor++;
						}
					}
					color /= nrcolor;
					tex.Set(nu, nv, color);
					nv += nTexelSize;
				}
				nu += nTexelSize;
				nv = 0;
			}
			m_MipMaps.push_back(tex);
			printf("Mipmap%d完成\n", l);
			//
			tex.Save(std::format("obj/mipmaps/mipmaps{}.tga", l));
		}
	}

	void Texture::SetTexture(Texture texture, int level)
	{
		if (level < 0 || level > m_MipMaps.size())
		{
			static_assert(1);
			printf("Texture Assert:\nMipMap没有分配足够空间!\n");
		}

		if (level == 0)
		{
			m_TGAImage.clear();
			m_TGAImage = texture.GetTGAImage();
			m_Width = texture.GetWidth();
			m_Height = texture.GetHeight();
			return;
		}

		m_MipMaps[level - 1] = texture;
	}

	Vector3f Texture::BilinearSample(float u, float v)
	{
		//只考虑正方形texture
		if (m_Width != m_Height)
		{
			printf("只支持正方形贴图！\n");
			return { 0, 0, 0 };
		}

		int tu = (int)(u * (m_Width - 1));
		int tv = (int)(v * (m_Height - 1));
		TGAColor m_TAGColor;
		Vector3f color = { 0, 0, 0 };

		int n = m_Width;
		float texelSize = 1.0f / (float)(n - 1);
		float u0 = (float)tu / (float)(n - 1);
		float v0 = (float)tv / (float)(n - 1);
		float u1 = std::fmin(u0 + texelSize, 1.0);
		float v1 = v0;
		float u2 = u0;
		float v2 = std::fmin(v0 + texelSize, 1.0);
		float u3 = std::fmin(u0 + texelSize, 1.0);
		float v3 = std::fmin(v0 + texelSize, 1.0);

		tu = (int)(u0 * (m_Width - 1));
		tv = (int)(v0 * (m_Height - 1));
		m_TAGColor = m_TGAImage.get(tu, tv);
		Vector3f color0 =
		{ m_TAGColor.bgra[2] / 255.0f, m_TAGColor.bgra[1] / 255.0f, m_TAGColor.bgra[0] / 255.0f };

		tu = (int)(u1 * (m_Width - 1));
		tv = (int)(v1 * (m_Height - 1));
		m_TAGColor = m_TGAImage.get(tu, tv);
		Vector3f color1 =
		{ m_TAGColor.bgra[2] / 255.0f, m_TAGColor.bgra[1] / 255.0f, m_TAGColor.bgra[0] / 255.0f };

		tu = (int)(u2 * (m_Width - 1));
		tv = (int)(v2 * (m_Height - 1));
		m_TAGColor = m_TGAImage.get(tu, tv);
		Vector3f color2 =
		{ m_TAGColor.bgra[2] / 255.0f, m_TAGColor.bgra[1] / 255.0f, m_TAGColor.bgra[0] / 255.0f };

		tu = (int)(u3 * (m_Width - 1));
		tv = (int)(v3 * (m_Height - 1));
		m_TAGColor = m_TGAImage.get(tu, tv);
		Vector3f color3 =
		{ m_TAGColor.bgra[2] / 255.0f, m_TAGColor.bgra[1] / 255.0f, m_TAGColor.bgra[0] / 255.0f };

		float s = (u - u0) / (u1 - u0);
		float t = (v - v0) / (v2 - v0);

		Vector3f color01 = Vector3f::Interpolate(color0, color1, s);
		Vector3f color23 = Vector3f::Interpolate(color2, color3, s);
		color = Vector3f::Interpolate(color01, color23, t);

		
		return color;
	}

	CubeMap::CubeMap()
	{
		m_CubeMap.clear();
	}

	CubeMap::CubeMap(Texture posx, Texture posy, Texture posz, Texture negx, Texture negy, Texture negz)
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
		
	}

	void CubeMap::SetCubeMap(Texture posx, Texture posy, Texture posz, Texture negx, Texture negy, Texture negz)
	{
		m_CubeMap.clear();
		m_CubeMap.push_back(posx);
		m_CubeMap.push_back(posy);
		m_CubeMap.push_back(posz);
		m_CubeMap.push_back(negx);
		m_CubeMap.push_back(negy);
		m_CubeMap.push_back(negz);
	}

	void CubeMap::SetCubeMap(Texture texture, int index)
	{
		if (index > 5 || index < 0) return;

		m_CubeMap[index] = texture;
	}

	void CubeMap::SetCubeMap(std::vector<Texture> textures, int level)
	{
		for (int i = 0; i < std::min((int)textures.size(), 6); i++)
		{
			m_CubeMap[i].SetTexture(textures[i], level);
		}
	}

	Vector3f CubeMap::Sample(const Vector3f& dir)
	{
		//get face
		int face = GetFace(dir);

		//get uv
		Vector2 uv = GetUV(face, dir);

		//sample

		if (m_CubeMap.empty()) return Vector3f();

		return (m_CubeMap[face].Sample(uv.u, uv.v));
	}

	Vector3f CubeMap::TrilinearSample(const Vector3f& dir, float lod)
	{
		//get face
		int face = GetFace(dir);

		//get uv
		Vector2 uv = GetUV(face, dir);

		//sample

		if (m_CubeMap.empty()) return Vector3f();

		//+0.0001自动开启双线性插值
		return (m_CubeMap[face].SampleLod(uv.u, uv.v, lod + 0.0001));
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
	* 首先定义一条射线：起始点p0和方向d，那么射线上任意一点p=p0 + td。
	* 平面的定义则通过一个法向n和一个顶点来实现，对于平面上的所有点p，p·n = d，d是一个常数。
	* 那么要计算它们之间的相交就不难了，若射线和平面相交，则必有：
	* (p0 + td)·n = d
	* p0·n + td·n = d
	* 最终可得：
	* t=(d - p0·n）/(d·n)。
	* 如果射线和平面互相平行，则d·n = 0，它们之间没有交点。d的计算则通过找平面上的一个点p，取p·n = d来实现。
	* 
	* 用t来计算
	* 假设在一个单位1的立方体里，从立方体中心除法
	* uv的范围在0 - 1， 而立方体是-1 - 1 要进行转换
	* 
	* 
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
		case 5://negz : u = -x, v = +y
			t = 1.0f / -dir.z;
			u = (-dir.x * t + 1.0f) * 0.5f;
			v = (dir.y *t + 1.0f) * 0.5f;
			break;
		default:
			break;
		}
		return Vector2(u, v);
	}

	void CubeMap::GenerateMipMap(int maxLevel)
	{
		for (auto& tex : m_CubeMap)
			tex.GenerateMipMap(maxLevel);
	}

	void CubeMap::SetSampleMode(SampleMode mode)
	{
		for (auto& tex : m_CubeMap)
			tex.SetSampleMode(mode);
	}

}