#include "hrpch.h"
#include "WindowsInput.h"
#include "Hare/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Hare
{
	// Since the project works only on windows we can create the singleton instance here.
	Input* Input::s_Instance = new WindowsInput();

	/*
	Those methods dispatch the inputs. For now they handle inputs using GLFW
	*/

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		// We want a GLFWwindow, this is the reason of static cast.
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window , keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		// We want a GLFWwindow, this is the reason of static cast.
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		// We want a GLFWwindow, this is the reason of static cast.
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xPos = 0;
		double yPos = 0;

		glfwGetCursorPos(window, &xPos, &yPos);

		// Return std::pair<float, float>. So we are returning two values in the same function.
		return { (float)xPos, (float)yPos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto[x, y] = GetMousePositionImpl();

		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto[x, y] = GetMousePositionImpl();

		return y;
	}
}