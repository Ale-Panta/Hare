#pragma once

#ifdef HR_PLATFORM_WINDOWS
	#ifdef HR_BUILD_DLL
		
		// https://docs.microsoft.com/en-us/cpp/cpp/dllexport-dllimport?view=msvc-160
		// This macro should be active when build Hare library.
		#define HARE_API __declspec(dllexport) 

	#else
		
		// https://docs.microsoft.com/en-us/cpp/cpp/dllexport-dllimport?view=msvc-160
		// This macro should be active when build Sandbox library.
		#define HARE_API __declspec(dllimport)

	#endif
#else
	#error Hare only supports windows!
#endif

#ifdef  HR_ENABLE_ASSERTS

	#define HR_ASSERT(x, ...) { if(!(x)) { HR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HR_CORE_ASSERT(x, ...) { if(!(x)) { HR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#else
	
	#define HR_ASSERT(x, ...)
	#define HR_CORE_ASSERT(x, ...)
	
#endif

/// <summary>
/// Macro used to filter events in categories.
/// Multiple categories are allowed.
/// <para>
/// <see cref="$(SolutionDir)Hare/Hare/Events/Event.h"/>
/// </para>
/// </summary>
#define BIT(x) (1 << x)

/// <summary>
/// Used to bind events. Used in particular
/// in ImGuiLayer.
/// <para>
/// <see cref="$(SolutionDir)Hare/Hare/ImGui/ImGuiLayer.h"/>
/// </para>
/// </summary>
#define HR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)