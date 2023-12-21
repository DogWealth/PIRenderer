#pragma once
#include "TGAImage.h"
#include <string>
#include "Vector.h"
namespace PIRenderer {
	class Texture
	{
	public:
		Texture() {};
		Texture(const std::string& filepath);
		~Texture();

		void LoadTexture(const std::string& filepath);
		Vector3f* Sample(float u, float v);

	private:
		TGAImage* m_TGAImage;
		Vector3f* m_TextureColor;
		TGAColor m_TAGColor;
		int m_Width;
		int m_Height;
	};
}

