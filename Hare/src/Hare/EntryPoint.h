#pragma once

#ifdef HR_PLATFORM_WINDOWS

/// <summary>
/// Somewhere we're going to have something that return Hare application pointer.
/// It's a function that we will define somewhere else which will actually return
/// that application for us.
/// </summary>
/// <returns></returns>
/// <see cref="$(SolutionDir)Hare/src/Hare/Application.h"/>
/// <see cref="$(SolutionDir)Sandbox/src/SandboxApp.cpp"/>
extern Hare::Application* Hare::CreateApplication();

// Create our Hare application for us. Entry point for both engine and application.
int main(int argc, char** argv)
{
	auto app = Hare::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif 