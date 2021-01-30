#include "hrpch.h"
#include "SubTexture2D.h"

using namespace glm;

namespace Hare
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& minBound, const glm::vec2& maxBound)
		: m_Texture(texture)
	{
		m_TextCoord[0] = vec2(minBound.x, minBound.y);
		m_TextCoord[1] = vec2(maxBound.x, minBound.y);
		m_TextCoord[2] = vec2(maxBound.x, maxBound.y);
		m_TextCoord[3] = vec2(minBound.x, maxBound.y);
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{
		vec2 minBound = vec2((coords.x * cellSize.x) / texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight());
		vec2 maxBound = vec2(((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight());

		return CreateRef<SubTexture2D>(texture, minBound, maxBound);
	}

}