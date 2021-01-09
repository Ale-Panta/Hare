#include <Hare.h>

////////////////////////////////////////
//////////////////Test//////////////////
////////////////////////////////////////

/*
This is just an example of how to create new custom layers.
*/
class ExampleLayer : public Hare::Layer 
{
public:
	ExampleLayer()
		: Layer("Example") { }

	void OnUpdate() override 
	{
		HR_INFO("ExampleLayer::Update");
	}

	void OnEvent(Hare::Event& event) override
	{
		HR_TRACE("{0}", event);
	}
};

////////////////////////////////////////
/////////Client Application/////////////
////////////////////////////////////////

class Sandbox : public Hare::Application 
{
public:
	Sandbox()
	{
		// Uncomment the line below to see how react the custom layer in console.
		//PushLayer(new ExampleLayer());

		PushOverlay(new Hare::ImGuiLayer());
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