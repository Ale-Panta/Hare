#pragma once

#include "Hare/Core/Base.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"

namespace Hare
{
	/*
	All members are static because inputs are global. We currently don't want
	to process inputs per window. It's a Singleton.
	Inputs are implemented per platform, so we declare here a sort of interface.
	*/
	class Input 
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}