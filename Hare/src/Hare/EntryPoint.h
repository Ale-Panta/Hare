#pragma once

#ifdef HR_PLATFORM_WINDOWS

/// <summary>
/// Somewhere we're going to have something that return Hare application pointer.
/// It's a function that we will define somewhere else which will actually return
/// that application for us.
/// <para>
/// <see cref="$(SolutionDir)Hare/src/Hare/Application.h"/>, 
/// <see cref="$(SolutionDir)Sandbox/src/SandboxApp.cpp"/>
/// </para>
/// </summary>
extern Hare::Application* Hare::CreateApplication();

// Create our Hare application for us. Entry point for both engine and application.
int main(int argc, char** argv)
{
	Hare::Log::Init();					// For testing purpose.
	HR_CORE_WARN("Initialize Log!");	// For testing purpose.
	HR_INFO("Hello!");					// For testing purpose.

	auto app = Hare::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif 