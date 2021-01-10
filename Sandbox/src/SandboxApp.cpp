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
		: Layer("Example") 
	{
	}

	void OnUpdate() override 
	{
		if (Hare::Input::IsKeyPressed(HR_KEY_TAB))
			HR_TRACE("[POOL] Tab key is pressed.");
	}

	void OnEvent(Hare::Event& event) override
	{
		if (event.GetEventType() == Hare::EventType::KeyPressed) 
		{
			Hare::KeyPressedEvent& e = (Hare::KeyPressedEvent&)event;

			if (e.GetKeyCode() == HR_KEY_TAB) {
				HR_TRACE("[EVENT] Tab key is pressed.");
			}
			HR_TRACE("[EVENT] {0}", (char)e.GetKeyCode());
		}
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
		PushLayer(new ExampleLayer());

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