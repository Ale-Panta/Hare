#include "hrpch.h"
#include "Hare/Core/Window.h"

#if defined(HR_PLATFORM_WINDOWS)
	#include "Platform/Window/WindowsWindow.h"
#elif defined(HR_PLATFORM_LINUX)
	#include "Platform/Linux/LinuxWindow.h"
#endif

namespace Hare
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#if defined(HR_PLATFORM_WINDOWS)
		return CreateScope<WindowsWindow>(props);
#elif defined(HR_PLATFORM_LINUX)
		return CreateScope<LinuxWindow>(props);
#else
		HR_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif

	}
}