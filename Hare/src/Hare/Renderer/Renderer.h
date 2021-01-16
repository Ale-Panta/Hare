#pragma once

#include "Shader.h"
#include "RenderCommand.h"
#include "OrthographicCamera.h"

namespace Hare
{
	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera); // TODO: Add parameters in order to renderer the scene
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4 transfrom = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}