workspace "Hare"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Hare/vendor/GLFW/include"
IncludeDir["Glad"] = "Hare/vendor/Glad/include"
IncludeDir["ImGui"] = "Hare/vendor/imgui"

include "Hare/vendor/GLFW"
include "Hare/vendor/Glad"
include "Hare/vendor/imgui"


project "Hare"
	location "Hare"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir  ("bin/" .. outputdir .. "/%{prj.name}")
	objdir  ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hrpch.h"
	pchsource "Hare/src/hrpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"HR_PLATFORM_WINDOWS",
			"HR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "HR_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HR_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "HR_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir  ("bin/" .. outputdir .. "/%{prj.name}")
	objdir  ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Hare/vendor/spdlog/include",
		"Hare/src"
	}

	links
	{
		"Hare"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"HR_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "HR_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "HR_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "HR_DIST"
		runtime "Release"
		optimize "On"