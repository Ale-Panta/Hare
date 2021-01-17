#include "hrpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

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

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transfrom)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);	// TODO: this should be run per shader.
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transfrom);	// This run per object.

		// mi->Bind();
		vertexArray->Bind();
		RenderCommand::DrawIndex(vertexArray);
	}
}