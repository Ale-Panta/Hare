#pragma once

#include "Hare.h"
#include "Level.h"
#include <imgui/imgui.h>

class GameLayer : public Hare::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Hare::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hare::Event& e) override;

	bool OnMouseButtonPressed(Hare::MouseButtonPressedEvent& e);
	bool OnWindowResize(Hare::WindowResizeEvent& e);

private:
	void CreateCamera(uint32_t width, uint32_t height);

private:
	Hare::Scope<Hare::OrthographicCamera> m_Camera;
	Level m_Level;
	ImFont* m_Font;
	float m_Time = 0.0f;
	bool m_Blink = false;

	enum class GameState
	{
		Play,
		MainMenu,
		GameOver
	};

	GameState m_State = GameState::MainMenu;
};