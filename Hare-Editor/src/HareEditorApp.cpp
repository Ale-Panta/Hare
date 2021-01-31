#include <Hare.h>
#include <Hare/Core/EntryPoint.h>	// Declare the entry point only when it's needed.

#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "EditorLayer.h"

using namespace std;

namespace Hare
{
	class HareEditor : public Application 
	{
	public:
		HareEditor()
			: Application("Hare Editor")
		{
			// Uncomment the line below to see how react the custom layer in console.
			// PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}

		~HareEditor()
		{

		}

	};

	/// <summary>
	/// Definition of Hare::CreateApplication.
	/// </summary>
	/// <returns>Hare::Application*</returns>
	/// <see cref="$(SolutionDir)Hare/src/Hare/EntryPoint.h"/>
	/// <see cref="$(SolutionDir)Hare/src/Hare/Application.h"/>
	Application* CreateApplication()
	{
		return new HareEditor();
	}
}