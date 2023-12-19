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
		Vector3f Sample(float u, float v) const;

	private:
		TGAImage* m_TGAImage;
		int m_Width;
		int m_Height;
	};
}

