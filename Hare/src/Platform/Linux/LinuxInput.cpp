//15-02-2021 Diego Cugno diego@cugno.org

#ifndef HR_PLATFORM_LINUX
	#ifndef __linux
		#warn "Compiler reports that the current compilation target isn't Linux. This file should be excluded from a compilation target that isn't Linux, UNIX, or POSIX."
	#endif
	#error "HR_PLATFORM_LINUX is not defined. Linux or a POSIX system is required to compile this file. Please define macro before compiling!"
#endif

#include "hrpch.h"
#include "Hare/Core/Input.h"
#include "Hare/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Hare
{
    bool Input::IsKeyPressed(const KeyCode keycode)
	{
        GLFWwindow* const window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        
        const int state = glfwGetKey(window, keycode);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(const MouseCode button)
	{
        GLFWwindow* const window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        const int state = glfwGetMouseButton(window, button);
        
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::GetMousePosition()
	{
        GLFWwindow* const window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        double xPos = 0;
        double yPos = 0;

        glfwGetCursorPos(window, &xPos, &yPos);

        return { (float)xPos, (float)yPos };
    }

    float Input::GetMouseX()
	{
		auto[x, y] = GetMousePosition();

		return x;
	}

	float Input::GetMouseY()
	{
		auto[x, y] = GetMousePosition();

		return y;
	}
}