#ifndef HR_PLATFORM_WINDOWS
	#ifndef _WIN64
		#warn "Compiler reports that the current compilation target isn't Win64. This file should be excluded from a compilation target that isn't Win64."
	#endif
	#error "HR_PLATFORM_WINDOWS is not defined. Win64 system is required to compile this file. Please define macro before compiling!"
#endif

#pragma once

#include "Hare/Core/Window.h"
#include "Hare/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace  Hare
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}