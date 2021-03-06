#pragma once

#include "Hare/Renderer/Texture.h"

#include <glad/glad.h>

namespace Hare
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

	public:
		inline virtual uint32_t GetWidth() const override { return m_Width; }
		inline virtual uint32_t GetHeight() const override { return m_Height; }
		inline virtual uint32_t GetRendererID() const override { return m_RendererID; }
		virtual void Bind(uint32_t slot = 0) const override;
		void SetData(void* data, uint32_t size) override;
		inline virtual bool operator==(const Texture& other) const override { return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID; }

	private:
		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat;
		GLenum m_DataFormat;
	};
}