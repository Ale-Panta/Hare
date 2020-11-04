#pragma once

#include "Core.h"
#include "Window.h"
#include "Events/Event.h"

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

	private:
		std::unique_ptr<Window> m_Window;
		bool m_IsRunning = true;
	};


	/// <summary>
	/// To be defined in the CLIENT
	/// </summary>
	/// <see cref="$(SolutionDir)Hare/src/Hare/EntryPoint.h"/>
	/// <see cref="$(SolutionDir)Sandbox/src/SandboxApp.cpp"/>
	Application* CreateApplication();
}

