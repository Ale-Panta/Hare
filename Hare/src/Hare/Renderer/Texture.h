#pragma once

#include "Hare/Core/Base.h"

#include <string>

namespace Hare
{
	// In the future the texture asset will be a custom Hare asset.
	// For now we remain with .png format.

	class Texture
	{
	public:
		virtual ~Texture() = default;

	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;

		// Get a pointer to block of memory and upload it in the GPU.
		virtual void SetData(void* data, uint32_t size) = 0;
		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& filepath);

	};
}