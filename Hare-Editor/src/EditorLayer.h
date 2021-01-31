#pragma once
#include "Hare.h"

namespace Hare
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		OrthographicCameraController m_CameraController;

		// Temp
		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;
		Ref<Texture2D> m_SpreadSheet;
		glm::vec4 m_Color = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);

		// Sheet
		Ref<SubTexture2D> m_TextureStair;
		Ref<SubTexture2D> m_TextureBarrel;
		Ref<SubTexture2D> m_TextureTree;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize;

		// Framebuffer
		Ref<Framebuffer> m_Framebuffer;
	};
}