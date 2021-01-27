#include "GameLayer.h"

#include <Hare.h>
#include <Hare/Core/EntryPoint.h>	// Declare the entry point only when it's needed.

////////////////////////////////////////
/////////Client Application/////////////
////////////////////////////////////////

class Sandbox : public Hare::Application 
{
public:
	Sandbox()
	{
		// Uncomment the line below to see how react the custom layer in console.
		// PushLayer(new ExampleLayer());
		PushLayer(new GameLayer());
	}

	~Sandbox()
	{

	}

};

/// <summary>
/// Definition of Hare::CreateApplication.
/// </summary>
/// <returns>Hare::Application*</returns>
/// <see cref="$(SolutionDir)Hare/src/Hare/EntryPoint.h"/>
/// <see cref="$(SolutionDir)Hare/src/Hare/Application.h"/>
Hare::Application* Hare::CreateApplication()
{
	return new Sandbox();
}