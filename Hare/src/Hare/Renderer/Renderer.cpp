#include "hrpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer2D.h"

namespace Hare
{
	Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();

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

	void Renderer::Init()
	{
		HR_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::ShutDown()
	{
		Renderer2D::ShutDown();
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

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
}