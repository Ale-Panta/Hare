#pragma once

#include "Core.h"

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
	};


	/// <summary>
	/// To be defined in the CLIENT
	/// </summary>
	/// <see cref="$(SolutionDir)Hare/src/Hare/EntryPoint.h"/>
	/// <see cref="$(SolutionDir)Sandbox/src/SandboxApp.cpp"/>
	Application* CreateApplication();
}

