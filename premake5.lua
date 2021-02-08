include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Hare"
	architecture "x86_64"
	startproject "HareInput"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Hare/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Hare/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Hare/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Hare/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Hare/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Hare/vendor/entt/include"

group "Dependencies"
	include "vendor/premake"
	include "Hare/vendor/GLFW"
	include "Hare/vendor/Glad"
	include "Hare/vendor/imgui"
group ""

include "Hare"
include "Sandbox"
include "HareInput"
