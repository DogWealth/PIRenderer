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

	Vector3f Texture::Sample(float u, float v) const
	{
		int tu = (int)(u * (m_Width - 1));
		int tv = (int)(v * (m_Height - 1));

		TGAColor color = m_TGAImage->get(tu, tv);
		return Vector3f(color.bgra[2], color.bgra[1], color.bgra[0]) / 255.0f;
	}

}