#ifdef HR_PLATFORM_LINUX

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

#endif