#include "GameLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Hare;
using namespace std;

GameLayer::GameLayer()
	: Layer("GameLayer")
{
	auto& window = Hare::Application::Get().GetWindow();
	CreateCamera(window.GetWidth(), window.GetHeight());

	Random::Init();
}

void GameLayer::OnAttach()
{
	m_Level.Init();

	ImGuiIO io = ImGui::GetIO();
	m_Font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-regular.ttf", 120.f);
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Hare::TimeStep ts)
{
	m_Time += ts;

	if ((int)(m_Time * 10.0f) % 8 > 4)
		m_Blink = !m_Blink;

	if (m_Level.IsGameOver())
		m_State = GameState::GameOver;

	const auto& playerPos = m_Level.GetPlayer().GetPosition();
	m_Camera->SetPosition({ playerPos.x, playerPos.y, 0.0f });

	if (m_State == GameState::Play)
		m_Level.OnUpdate(ts);

	// Render
	Hare::RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	Hare::RenderCommand::Clear();

	Hare::Renderer2D::BeginScene(*m_Camera);
	m_Level.OnRenderer();
	Hare::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
	switch (m_State)
	{
	case GameLayer::GameState::Play:
	{
		uint32_t playerScore = m_Level.GetPlayer().GetScore();
		string scoreStr = string("Score: ") + to_string(playerScore);
		ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
		break;

	}
	case GameLayer::GameState::MainMenu:
	{

		auto pos = ImGui::GetWindowPos();
		auto width = Application::Get().GetWindow().GetWidth();
		auto height = Application::Get().GetWindow().GetHeight();

		pos.x += width * 0.5f - 300.0f;
		pos.y += 50.0f;

		if (m_Blink)
			ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to play!");
		break;
	}

	case GameLayer::GameState::GameOver:
	{

		auto pos = ImGui::GetWindowPos();
		auto width = Application::Get().GetWindow().GetWidth();
		auto height = Application::Get().GetWindow().GetHeight();

		pos.x += width * 0.5f - 300.0f;
		pos.y += 50.0f;

		if (m_Blink)
			ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to play!");

		pos.x += 200.0f;
		pos.y += 150.0f;
		uint32_t playerScore = m_Level.GetPlayer().GetScore();
		string scoreStr = string("Score: ") + to_string(playerScore);
		ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
		break;
	}
	}
}

void GameLayer::OnEvent(Hare::Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>(HR_BIND_EVENT_FN(GameLayer::OnWindowResize));
	dispatcher.Dispatch<MouseButtonPressedEvent>(HR_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
}

bool GameLayer::OnMouseButtonPressed(Hare::MouseButtonPressedEvent& e)
{
	if (m_State == GameState::GameOver)
		m_Level.Reset();

	m_State = GameState::Play;
	return false;
}

bool GameLayer::OnWindowResize(WindowResizeEvent& e)
{
	CreateCamera(e.GetWidth(), e.GetHeigth());
	return false;
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
	// We need to resize the camera manually if resize window.

	float aspectRatio = (float)width / (float)height;

	float camWidth	= 10.0f;
	float bottom	= -camWidth;
	float top		= camWidth;
	float left		= bottom * aspectRatio;
	float right		= top * aspectRatio;

	m_Camera = CreateScope<OrthographicCamera>(left, right, bottom, top);
}

