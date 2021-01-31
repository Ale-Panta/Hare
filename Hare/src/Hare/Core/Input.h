#pragma once

#include "Hare/Core/Core.h"

namespace Hare
{
	/*
	All members are static because inputs are global. We currently don't want
	to process inputs per window. It's a Singleton.
	Inputs are implemented per platform, so we declare here a sort of interface.
	*/
	class HARE_API Input 
	{
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}