#pragma once

#include "Core.h"
#include "Window.h"
#include "Hare/LayerStack.h"
#include "Events/Event.h"
#include "Hare/Events/ApplicationEvent.h"

#include "Hare/ImGui/ImGuiLayer.h"

// Temp
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

namespace Hare
{
	/// <summary>
	/// Base application class (this will be inherited).
	/// </summary>
	class HARE_API Application 
	{
	public:
		Application();
		virtual ~Application();

		/// <summary>
		/// Run the application.
		/// </summary>
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_IsRunning = true;
		LayerStack m_LayerStack;

		// --- Begin Renderer 
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_ShaderSquare;
		std::shared_ptr<VertexArray> m_SquareVA;
		// --- End renderer


		static Application* s_Instance;
	};


	/// <summary>
	/// To be defined in the CLIENT
	/// </summary>
	/// <see cref="$(SolutionDir)Hare/src/Hare/EntryPoint.h"/>
	/// <see cref="$(SolutionDir)Sandbox/src/SandboxApp.cpp"/>
	Application* CreateApplication();
}

