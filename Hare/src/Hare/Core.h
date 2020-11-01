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
