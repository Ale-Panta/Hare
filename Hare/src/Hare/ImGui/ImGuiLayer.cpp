#include "hrpch.h"
#include "imgui.h"
#include "ImGuiLayer.h"
#include "Hare/Core/Application.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include <ImGuizmo.h>

// TEMP
#include <GLFW/glfw3.h>
#include <glad/glad.h>

using namespace ImGui;
using namespace ImGuizmo;

namespace Hare
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::OnAttach()
	{
		HR_PROFILE_FUNCTION();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		io.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Bold.ttf", 16.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/roboto/Roboto-Regular.ttf", 16.0f);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//StyleColorsClassic();

		// When viewport are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColor();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		HR_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}


	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		HR_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		NewFrame();
		BeginFrame();
	}

	void ImGuiLayer::End()
	{
		HR_PROFILE_FUNCTION();

		ImGuiIO& io = GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}


	void ImGuiLayer::SetDarkThemeColor()
	{
		auto& colors = GetStyle().Colors;

		colors[ImGuiCol_WindowBg]			= ImVec4(0.150f, 0.150f, 0.150f, 1.0f);

		// --- Headers ------------------------------------------------------------
		colors[ImGuiCol_Header]				= ImVec4(0.109f, 0.109f, 0.109f, 1.0f);
		colors[ImGuiCol_HeaderHovered]		= ImVec4(0.218f, 0.218f, 0.218f, 1.0f);
		colors[ImGuiCol_HeaderActive]		= ImVec4(0.218f, 0.218f, 0.218f, 1.0f);

		// --- Buttons ------------------------------------------------------------
		colors[ImGuiCol_Button]				= ImVec4(0.109f, 0.109f, 0.109f, 1.0f);
		colors[ImGuiCol_ButtonHovered]		= ImVec4(0.218f, 0.218f, 0.218f, 1.0f);
		colors[ImGuiCol_ButtonActive]		= ImVec4(0.218f, 0.218f, 0.218f, 1.0f);

		// --- Frame BG -----------------------------------------------------------
		colors[ImGuiCol_FrameBg]			= ImVec4(0.109f, 0.109f, 0.109f, 1.0f);
		colors[ImGuiCol_FrameBgHovered]		= ImVec4(0.218f, 0.218f, 0.218f, 1.0f);
		colors[ImGuiCol_FrameBgActive]		= ImVec4(0.218f, 0.218f, 0.218f, 1.0f);

		// --- Tabs ---------------------------------------------------------------
		colors[ImGuiCol_Tab]				= ImVec4(0.109f, 0.109f, 0.109f, 1.0f);
		colors[ImGuiCol_TabHovered]			= ImVec4(0.378f, 0.307f, 0.409f, 1.0f);
		colors[ImGuiCol_TabActive]			= ImVec4(0.150f, 0.150f, 0.150f, 1.0f);
		colors[ImGuiCol_TabUnfocused]		= ImVec4(0.150f, 0.150f, 0.150f, 1.0f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.150f, 0.150f, 0.150f, 1.0f);

		// --- Titles --------------------------------------------------------------
		colors[ImGuiCol_TitleBg]			= ImVec4(0.109f, 0.109f, 0.109f, 1.0f);
		colors[ImGuiCol_TitleBgActive]		= ImVec4(0.378f, 0.307f, 0.409f, 1.0f);
		colors[ImGuiCol_TitleBgCollapsed]	= ImVec4(0.378f, 0.307f, 0.409f, 1.0f);

		// --- PopUp ---------------------------------------------------------------
		colors[ImGuiCol_PopupBg]			= ImVec4(0.150f, 0.150f, 0.150f, 1.0f);

		// --- Border --------------------------------------------------------------
		colors[ImGuiCol_Border]				= ImVec4(0.378f, 0.307f, 0.409f, 1.0f);

		// --- Menu Bar ------------------------------------------------------------
		colors[ImGuiCol_MenuBarBg]			= ImVec4(0.109f, 0.109f, 0.109f, 1.0f);

		// --- Text ----------------------------------------------------------------
		colors[ImGuiCol_TextSelectedBg]		= ImVec4(0.218f, 0.218f, 0.218f, 1.0f);
	}

}