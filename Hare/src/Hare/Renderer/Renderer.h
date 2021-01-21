#pragma once

#include "Shader.h"
#include "RenderCommand.h"
#include "OrthographicCamera.h"

namespace Hare
{
	/*
	By default Hare engine it's a 3D renderer. So When you read Renderer it means 3D.
	We support also 2D but we need a different API. Renderer2D comes in to this job.
	*/

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera); // TODO: Add parameters in order to renderer the scene
		static void EndScene();

		static void Init();
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transfrom = glm::mat4(1.0f));

		static void OnWindowResize(uint32_t width, uint32_t height);
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> m_SceneData;
	};
}