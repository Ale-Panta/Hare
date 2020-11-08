#include <Hare.h>

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

class Sandbox : public Hare::Application 
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
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