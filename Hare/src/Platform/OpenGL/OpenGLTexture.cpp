#include "hrpch.h"
#include "stb_image.h"
#include "OpenGLTexture.h"

using namespace std;

namespace Hare
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		HR_PROFILE_FUNCTION();

		// Read channels and set image's data format and internal format.
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);	// Allocate memory in the GPU. How openGL is going to store our texture.

		// Set texture parameters.
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const string& path)
		: m_Path(path)
	{
		HR_PROFILE_FUNCTION();

		int width;
		int height;
		int channels;

		stbi_set_flip_vertically_on_load(true);

		stbi_uc* data = nullptr;
		{
			HR_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		HR_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		// Read channels and set image's data format and internal format.
		GLenum internalFormat = 0;
		GLenum dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		HR_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);	// Allocate memory in the GPU. How openGL is going to store our texture.

		// Set texture parameters.
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Upload our texture in the GPU. Determine the format of our data.
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		// Once the data are uploaded tp the GPU we no longer need to retain it in the CPU memory.
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		HR_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}
	
	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		HR_PROFILE_FUNCTION();

		uint32_t bytesPerChannel = m_DataFormat == GL_RGBA ? 4 : 3;
		HR_CORE_ASSERT(size == m_Width * m_Height * bytesPerChannel, "Data must be the entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		HR_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}
}