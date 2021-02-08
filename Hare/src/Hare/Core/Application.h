#pragma once

#include "Core.h"
#include "Window.h"
#include "Hare/Events/Event.h"
#include "Hare/Core/TimeStep.h"
#include "Hare/Core/LayerStack.h"
#include "Hare/ImGui/ImGuiLayer.h"
#include "Hare/Events/ApplicationEvent.h"
#include "Hare/Renderer/OrthographicCamera.h"

namespace Hare
{
	/// <summary>
	/// Base application class (this will be inherited).
	/// </summary>
	class Application 
	{
	public:
		Application(const std::string& name = "Hare App");
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Close();

		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_IsRunning = true;
		bool m_Minimize = false;

		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;	// Time that took to render the last frame.

		static Application* s_Instance;
	};


	/// <summary>
	/// To be defined in the CLIENT
	/// </summary>
	/// <see cref="$(SolutionDir)Hare/src/Hare/EntryPoint.h"/>
	/// <see cref="$(SolutionDir)Sandbox/src/SandboxApp.cpp"/>
	Application* CreateApplication();
}

