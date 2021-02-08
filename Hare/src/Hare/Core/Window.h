#pragma once

#include "hrpch.h"
#include "Hare/Core/Core.h"
#include "Hare/Events/Event.h"

namespace Hare
{
	struct WindowProps 
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Hare Engine",
			uint32_t width = 1600,
			uint32_t height = 900)
			: Title(title), Width(width), Height(height) { }
	};

	/// <summary>
	/// Interface representing a desktop system based Window.
	/// Mobiles instead of window has surface.
	/// Abstract representation of a window.
	/// It is platform indipendent.
	/// </summary>
	class Window 
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		// All the stuff below are pure virtual because
		// they has to be implemented per-platform.

		virtual ~Window() { }

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		/*
		Expose our window instance (GLFW window for now) to the client. It is void pointer
		because potentially it doesn't have to return GLFW window, it could be anything.
		*/
		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}