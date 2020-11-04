#include "hrpch.h"

#include "Application.h"

#include "Events/Event.h"
#include "Hare/Events/ApplicationEvent.h"
#include "Hare/Log.h"

namespace Hare
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		HR_TRACE(e);

		while (true);
	}
}