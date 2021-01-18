#include "hrpch.h"
#include "Input.h"
#include "Application.h"
#include "Hare/Renderer/Renderer.h"

#include <glfw/glfw3.h>

namespace Hare
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HR_CORE_ASSERT(!s_Instance, "Application already exist!")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));


		// Iterate the layers backwords, because we want the latest layer 
		// to blocks input first (if it handle it).
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);

			if (e.Handled)
				break;			// Break as soon the event is handled by the layer.
		}
	}

	void Application::Run()
	{
		while (m_IsRunning)
		{
			// Delta time calculation.
			float time = (float)glfwGetTime();	// TODO: Platform::GetTime();
			TimeStep timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timeStep);

			// ------ Start ImGui render -------
			// Will be rendered in render tread.
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			// ------ End ImGui render ---------

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}
}