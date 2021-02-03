#pragma once

#include <memory>

#ifdef HR_PLATFORM_WINDOWS
	#if HR_DYNAMIC_LINK

		// We don't use dynamic libraries anymore...
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
		#define HARE_API
#endif

#else
	#error Hare only supports windows!
#endif

#ifdef HR_DEBUG
	#define HR_ENABLE_ASSERTS
#endif

#ifdef HR_ENABLE_ASSERTS

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
#define HR_BIND_EVENT_FN(fn) [this](auto&&... args)->decltype(auto){ return this->fn(std::forward<decltype(args)>(args)...);}


namespace Hare
{
	/*
	We change how unique and shared pointers looks like.
	Every object in the scene will be 99% of the time a shared pointer,
	mostly for this reason: it's multithread safety because it use
	atomic increment and decrement. We need this kind of safety because 
	the rendering calculation is done in a different thread.
	*/

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}