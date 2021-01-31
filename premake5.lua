workspace "Hare"
	architecture "x64"
	startproject "Hare-Editor"

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
IncludeDir["glm"] = "Hare/vendor/glm"
IncludeDir["stb_image"] = "Hare/vendor/stb_image"

group "Dependecies"
	include "Hare/vendor/GLFW"
	include "Hare/vendor/Glad"
	include "Hare/vendor/imgui"
group ""

project "Hare"
	location "Hare"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir  ("bin/" .. outputdir .. "/%{prj.name}")
	objdir  ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hrpch.h"
	pchsource "Hare/src/hrpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HR_PLATFORM_WINDOWS",
			"HR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "HR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HR_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Hare/src",
		"Hare/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Hare"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HR_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "HR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HR_DIST"
		runtime "Release"
		optimize "on"

project "Hare-Editor"
	location "Hare-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Hare/src",
		"Hare/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Hare"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HR_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "HR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HR_DIST"
		runtime "Release"
		optimize "on"