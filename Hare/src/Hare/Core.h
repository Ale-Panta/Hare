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


// Macro used to filter events in categories.
// Multiple categories are allowed.
/// <"see cref="$(SolutionDir)Hare/Hare/Events/Event.h"/>
#define BIT(x) (1 << x)