#pragma once
#include "Hare.h"
#include "Panels/SceneHierarchyPanel.h"

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
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();

	private:
		OrthographicCameraController m_CameraController;

		// Temp
		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;

		// Sheet
		Ref<Texture2D> m_SpreadSheet;
		Ref<SubTexture2D> m_TextureStair;
		Ref<SubTexture2D> m_TextureBarrel;
		Ref<SubTexture2D> m_TextureTree;

		Ref<Scene> m_ActiveScene;
		Entity m_Square;
		Entity m_CameraEntity;
		Entity m_SecondCamera;
		bool m_PrimaryCamera = true;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = glm::vec2(0.0f, 0.0f);

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;

		// Gizmos
		int m_GizmoType = -1;	// None
		float m_SnapTranslationAndScaleValue = 0.5f;
		float m_SnapRotationValue = 45.0f;
	};
}