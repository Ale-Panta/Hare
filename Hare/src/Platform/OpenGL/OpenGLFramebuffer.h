#pragma once
#include "Hare/Renderer/Framebuffer.h"

namespace Hare
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(FramebufferSpecification specification);
		virtual ~OpenGLFramebuffer();

	public:
		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		//inline virtual uint32_t GetColorAttachmentRenderID(uint32_t index = 0) const override { HR_CORE_ASSERT(index < m_ColorAttachments.size()); return m_ColorAttachments[index]; }
		inline virtual uint32_t GetColorAttachmentRenderID(uint32_t index = 0) const override { return m_ColorAttachment; }
		inline virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }

	private:
		uint32_t m_RendererID = 0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecification;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureSpecification(FramebufferTextureFormat::None);

		//std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment = 0;
	};
}