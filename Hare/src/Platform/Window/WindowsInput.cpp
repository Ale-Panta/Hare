#include "hrpch.h"
#include "Hare/Core/Input.h"
#include "Hare/Core/Application.h"

#include <GLFW/glfw3.h>

using namespace std;

namespace Hare
{
	bool Input::IsKeyPressed(const KeyCode keycode)
	{
		// We want a GLFWwindow, this is the reason of static cast.
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetKey(window , keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		// We want a GLFWwindow, this is the reason of static cast.
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	pair<float, float> Input::GetMousePosition()
	{
		// We want a GLFWwindow, this is the reason of static cast.
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xPos = 0;
		double yPos = 0;

		glfwGetCursorPos(window, &xPos, &yPos);

		// Return pair<float, float>. So we are returning two values in the same function.
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