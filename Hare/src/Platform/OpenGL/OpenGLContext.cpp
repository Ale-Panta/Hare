#include "hrpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hare
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HR_CORE_ASSERT(m_WindowHandle, "Window handle is null")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HR_CORE_ASSERT(status, "Failed to initialize Glad!");

		HR_CORE_INFO("OpenGL Info");
		HR_CORE_INFO("	Vendor:		{0}", glGetString(GL_VENDOR));
		HR_CORE_INFO("	Renderer:	{0}", glGetString(GL_RENDERER));
		HR_CORE_INFO("	Version:	{0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}