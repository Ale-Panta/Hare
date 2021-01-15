#include "hrpch.h"
#include "Renderer.h"

namespace Hare
{
	void Renderer::BeginScene()
	{
		// Make sure that the shader we are using has the right uniforms,
		// wich cube map sample from... projection matric, view matrix,
		// lighting...
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndex(vertexArray);
	}
}