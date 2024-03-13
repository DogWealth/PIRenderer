#pragma once
#include "TGAImage.h"
#include "Vector.h"

#include <string>
#include <vector>
namespace PIRenderer {
	class Texture
	{
	public:
		Texture() {};
		Texture(const std::string& filepath);
		~Texture();

		void LoadTexture(const std::string& filepath);
		Vector3f Sample(float u, float v);

	private:
		TGAImage* m_TGAImage;
		int m_Width;
		int m_Height;
	};

	/*
	* cubemap
	* ×óÊÖ×ø±êÏµ
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
		CubeMap(Texture* posx, Texture* posy, Texture* posz, Texture* negx, Texture* negy, Texture* negz);
		~CubeMap() = default;

		void Clear();
		void DelteAllTextures();
		void SetCubeMap(Texture* posx, Texture* posy, Texture* posz, Texture* negx, Texture* negy, Texture* negz);
		void SetCubeMap(Texture* texture, int index);

		Vector3f Sample(const Vector3f& dir);

	private:
		int GetFace(const Vector3f& dir);
		Vector2 GetUV(int face, const Vector3f& dir);

	private:
		std::vector<Texture*> m_CubeMap;
	};
}

