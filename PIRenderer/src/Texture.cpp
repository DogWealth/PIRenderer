#include "Texture.h"

namespace PIRenderer {
	Texture::Texture(const std::string& filepath)
	{
		LoadTexture(filepath);

		m_Width = m_TGAImage->get_width();
		m_Height = m_TGAImage->get_height();

		m_TextureColor = new Vector3f();
	}

	Texture::~Texture()
	{
		delete m_TGAImage;
		delete m_TextureColor;
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

	Vector3f* Texture::Sample(float u, float v)
	{
		int tu = (int)(u * (m_Width - 1));
		int tv = (int)(v * (m_Height - 1));

		m_TAGColor = m_TGAImage->get(tu, tv);
		m_TextureColor->x = m_TAGColor.bgra[2] / 255.0f;
		m_TextureColor->y = m_TAGColor.bgra[1] / 255.0f;
		m_TextureColor->z = m_TAGColor.bgra[0] / 255.0f;
		return m_TextureColor;

	}

}