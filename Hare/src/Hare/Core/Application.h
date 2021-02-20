#pragma once

#include "Base.h"
#include "Hare/Core/Window.h"
#include "Hare/Events/Event.h"
#include "Hare/Core/TimeStep.h"
#include "Hare/Core/LayerStack.h"
#include "Hare/ImGui/ImGuiLayer.h"
#include "Hare/Events/ApplicationEvent.h"

int main(int argc, char** argv);

namespace Hare
{
	/// Base application class, all specialized application such as HareINput and Sandbox inherit from it.
	class Application 
	{
		friend int ::main(int argc, char** argv);

	public:
		Application(const std::string& name = "Hare App");
		virtual ~Application();

	public:
		// All event are dispatched and processed here.
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Close();

		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_IsRunning = true;
		bool m_Minimize = false;

		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
	};


	/// To be defined in the CLIENT
	Application* CreateApplication();
}

