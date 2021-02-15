#include "hrpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <iostream>
#include <GLFW/glfw3.h>
// --- Must be included in this order ---
#include <glad/glad.h>
#include <GL/gl.h>
// --------------------------------------

namespace Hare
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HR_CORE_ASSERT(m_WindowHandle, "Window handle is null")
	}

	void OpenGLContext::Init()
	{
		HR_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HR_CORE_ASSERT(status, "Failed to initialize Glad!");

		HR_CORE_INFO("OpenGL Info");
		HR_CORE_INFO("	Vendor:		{0}", glGetString(GL_VENDOR));
		HR_CORE_INFO("	Renderer:	{0}", glGetString(GL_RENDERER));
		HR_CORE_INFO("	Version:	{0}", glGetString(GL_VERSION));

	#ifdef HR_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		HR_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hazel require at least OpenGL version 4.5!");
	#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		HR_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}