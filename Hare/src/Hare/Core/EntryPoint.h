#pragma once

#include "Hare/Core/Base.h"

#if defined(HR_PLATFORM_WINDOWS) || defined(HR_PLATFORM_LINUX)

// Somewhere we're going to have something that return Hare application pointer.
// It's a function that we will define somewhere else which will actually return
// that application for us.
extern Hare::Application* Hare::CreateApplication();

// Create our Hare application for us. Entry point for both engine and application.
int main(int argc, char** argv)
{
	Hare::Log::Init();

	HR_PROFILE_BEGIN_SESSION("Startup", "HareProfile-Startup.json");
	auto app = Hare::CreateApplication();
	HR_PROFILE_END_SESSION();
	
	HR_PROFILE_BEGIN_SESSION("Runtime", "HareProfile-Runtime.json");
	app->Run();
	HR_PROFILE_END_SESSION();

	HR_PROFILE_BEGIN_SESSION("Shutdown", "HareProfile-Shutdown.json");
	delete app;
	HR_PROFILE_END_SESSION();

	return 0;
}

#endif 