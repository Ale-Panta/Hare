#pragma once
#include "Hare/Renderer/GraphicsContext.h"

// Forward declaration.
struct GLFWwindow;

namespace Hare
{
	class OpenGLContext : public GraphicsContext 
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}