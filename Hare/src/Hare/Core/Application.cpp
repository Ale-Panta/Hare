#include "hrpch.h"
#include "Hare/Core/Log.h"
#include "Hare/Core/Input.h"
#include "Hare/Core/Application.h"
#include "Hare/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

using namespace std;

namespace Hare
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		HR_PROFILE_FUNCTION();

		HR_CORE_ASSERT(!s_Instance, "Application already exist!")
		s_Instance = this;
		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(HR_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		HR_PROFILE_FUNCTION();

		Renderer::ShutDown();
	}

	void Application::PushLayer(Layer* layer)
	{
		HR_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		HR_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}


	void Application::Close()
	{
		m_IsRunning = false;
	}

	void Application::OnEvent(Event& e)
	{
		HR_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(HR_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HR_BIND_EVENT_FN(Application::OnWindowResize));


		// Iterate the layers backwards, because we want the latest layer 
		// to blocks input first (if it handle it).
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;			// Break as soon the event is handled by the layer.

			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		HR_PROFILE_FUNCTION();

		while (m_IsRunning)
		{
			HR_PROFILE_SCOPE("Runloop");

			// Delta time calculation.
			float time = (float)glfwGetTime();	// TODO: Platform::GetTime();
			DeltaTime timeStep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimize)
			{
				{
					HR_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timeStep);
				}

				// ------ Start ImGui render -------
				// Will be rendered in render tread.
				m_ImGuiLayer->Begin();
				{
					HR_PROFILE_SCOPE("LayerStack OnImGuiRenderer");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
				// ------ End ImGui render ---------
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_IsRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent & e)
	{
		HR_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeigth() == 0)
		{
			m_Minimize = true;
			return false;
		}

		m_Minimize = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeigth());

		return false;
	}
}