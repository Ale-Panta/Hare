#pragma once

#include "Texture.h"
#include <glm/glm.hpp>

namespace Hare
{
	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& minBound, const glm::vec2& maxBound);

		const Ref<Texture2D> GetTexture() const { return m_Texture; }
		const glm::vec2* GetTexCoord() const { return m_TextCoord; }

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = glm::vec2(1, 1));

	private:
		Ref<Texture2D> m_Texture;

		glm::vec2 m_TextCoord[4];
	};
}