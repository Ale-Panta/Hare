#include "hrpch.h"
#include "Renderer.h"

namespace Hare
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		// Make sure that the shader we are using has the right uniforms,
		// wich cube map sample from... projection matric, view matrix,
		// lighting...

		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4 transfrom)
	{
		shader->Bind();
		shader->UploadUniformMat4f("u_ViewProjection", m_SceneData->ViewProjectionMatrix);	// TODO: this should be run per shader.
		shader->UploadUniformMat4f("u_Transform", transfrom);	// This run per object.
		vertexArray->Bind();
		RenderCommand::DrawIndex(vertexArray);
	}
}