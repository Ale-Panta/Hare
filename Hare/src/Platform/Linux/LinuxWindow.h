#ifndef HR_PLATFORM_LINUX
	#ifndef __linux
		#warn "Compiler reports that the current compilation target isn't Linux. This file should be excluded from a compilation target that isn't Linux, UNIX, or POSIX."
	#endif
	#error "HR_PLATFORM_LINUX is not defined. Linux or a POSIX system is required to compile this file. Please define macro before compiling!"
#endif

#pragma once

#include "Hare/Core/Window.h"
#include "Hare/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Hare
{
    class LinuxWindow : public Window
    {
    public:
        LinuxWindow(const WindowProps& props);
        virtual ~LinuxWindow();

        void OnUpdate();

        inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

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