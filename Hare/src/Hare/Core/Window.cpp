#include "hrpch.h"
#include "Hare/Core/Window.h"

#ifdef HR_PLATFORM_WINDOWS
	#include "Platform/Window/WindowsWindow.h"
#endif

namespace Hare
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef HR_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		HR_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif // HR_PLATFORM_WINDOWS

	}
}