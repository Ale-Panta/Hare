#pragma once
#include "Hare/Renderer/Camera.h"
#include "Hare/Scene/Components.h"
#include "Hare/Renderer/Texture.h"
#include "Hare/Renderer/EditorCamera.h"
#include "Hare/Renderer/SubTexture2D.h"
#include "Hare/Renderer/OrthographicCamera.h"

namespace Hare
{
	// It's a wrapper over certain GPU render commands.
	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();
		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();

		// Draw quads in a single draw call.
		static void Flush();

		// Primitives. Are differentiated in draw sprite and draw quad. The reason is: we are able to draw something that is
		// not an entity, such as UI or environment elements that are not related to entity at all.

		// Add values to vertex buffer.
		static void DrawQuad(const glm::mat4& transfrom, const glm::vec4& color, int entityID = -1);
		// Add values to vertex buffer.
		static void DrawQuad(const glm::mat4& transfrom, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID = -1);
		// Add values to vertex buffer.
		static void DrawQuad(const glm::mat4& transfrom, const Ref<SubTexture2D>& subTexture, float tilingFactor, const glm::vec4& tintColor);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angleRad, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angleRad, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angleRad, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angleRad, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angleRad, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angleRad, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		// Use in editor only
		static void DrawSprite(const glm::mat4& transfrom, SpriteRendererComponent& spriteRendererComponent, int entityID);

		// Stats
		struct Statistics
		{
			uint32_t Drawcalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};
}