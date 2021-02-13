#pragma once

#pragma once

#include "Hare/Core/Base.h"

namespace Hare
{
	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth / Stencil
		DEPTH24STENCIL8,

		// Defaults
		Default = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		// Constructors
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat textureFormat)
			: TextureFormat(textureFormat)
		{ }

		// Member variables
		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;

		// TODO: filtering / wrap.
	};

	struct FramebufferAttachmentSpecification
	{
		// Constructors
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments)
		{
		}

		// Member variables
		std::vector<FramebufferTextureSpecification> Attachments;
	};


	// Framebuffer properties.
	struct FramebufferSpecification
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Samples = 1;
		FramebufferAttachmentSpecification Attachments;

		// Vulkan...
		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual uint32_t GetColorAttachmentRenderID(uint32_t index = 0) const = 0;
		virtual const FramebufferSpecification& GetSpecification() const = 0;
		static Ref<Framebuffer> Create(const FramebufferSpecification& specification);
	};
}