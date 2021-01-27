#pragma once

#include "hrpch.h"
#include "Hare/Core/Core.h"
#include "Hare/Events/Event.h"

namespace Hare
{
	struct WindowProps 
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Hare Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height) { }
	};

	/// <summary>
	/// Interface representing a desktop system based Window.
	/// Mobiles instead of window has surface.
	/// Abstract representation of a window.
	/// It is platform indipendent.
	/// </summary>
	class HARE_API Window 
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		// All the stuff below are pure virtual because
		// they has to be implemented per-platform.

		virtual ~Window() { }

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		/*
		Expose our window instance (GLFW window for now) to the client. It is void pointer
		because potentially it doesn't have to return GLFW window, it could be anything.
		*/
		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}