#pragma once
#include "Hare.h"

class Sandbox2D : public Hare::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Hare::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hare::Event& e) override;

private:
	Hare::OrthographicCameraController m_CameraController;

	// Temp
	Hare::Ref<Hare::Shader> m_FlatColorShader;
	Hare::Ref<Hare::VertexArray> m_SquareVA;
	glm::vec4 m_Color = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);
	Hare::Ref<Hare::Texture2D> m_Texture;
};