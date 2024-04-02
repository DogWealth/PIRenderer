#pragma once
#include "TGAImage.h"
#include "Vector.h"

#include <string>
#include <vector>
namespace PIRenderer {
	enum SampleMode {
		Nearest = 0, Bilinear, Trilinear
	};

	class Texture
	{
	public:
		Texture() {};
		Texture(int width, int height, int bytespp = 3);
		Texture(const std::string& filepath);
		~Texture();

		void LoadTexture(const std::string& filepath);
		Vector3f Sample(float u, float v);
		Vector3f Sample(const Vector2& texcoord);
		Vector3f SampleLod(float u, float v, float lod = 0);
		void Set(float u, float v, const Vector3f& c);

		bool Save(const std::string& filename);

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		const TGAImage& GetTGAImage() const { return m_TGAImage; }

		//只针对正方形texture
		void GenerateMipMap(int maxLevel);
		void SetSampleMode(SampleMode mode) { m_SampleMode = mode; }

		void SetTexture(Texture texture, int level);

	private:
		Vector3f BilinearSample(float u, float v);


	private:
		TGAImage m_TGAImage;
		int m_Width;
		int m_Height;
		std::vector<Texture> m_MipMaps;
		SampleMode m_SampleMode = Nearest;
	};

	/*
	* cubemap
	* 左手坐标系
	* index0 : posx
	* index1 : posy
	* index2 : posz
	* index3 : negx
	* index4 : negy
	* index5 : negz
	*/
	class CubeMap
	{
	public:
		CubeMap();
		CubeMap(Texture posx, Texture posy, Texture posz, Texture negx, Texture negy, Texture negz);
		~CubeMap() = default;

		void Clear();
		void DelteAllTextures();
		void SetCubeMap(Texture posx, Texture posy, Texture posz, Texture negx, Texture negy, Texture negz);
		void SetCubeMap(Texture texture, int index);
		void SetCubeMap(std::vector<Texture> textures, int level);

		Vector3f Sample(const Vector3f& dir);
		Vector3f TrilinearSample(const Vector3f& dir, float lod);

		int GetFace(const Vector3f& dir);
		Vector2 GetUV(int face, const Vector3f& dir);

		void GenerateMipMap(int maxLevel);
		void SetSampleMode(SampleMode mode);

		int GetWidth() const { return m_CubeMap[0].GetWidth(); }
		int GetHeight() const { return m_CubeMap[0].GetHeight(); }


	private:
		std::vector<Texture> m_CubeMap;
	};
}

