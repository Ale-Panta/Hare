#pragma once

#include "Hare/Core/Layer.h"

#include "Hare/Events/KeyEvent.h"
#include "Hare/Events/MouseEvent.h"
#include "Hare/Events/ApplicationEvent.h"

namespace Hare
{
	class HARE_API ImGuiLayer : public Layer 
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();	// Begin ImGui rendering
		void End();		// End ImGui rendering

	private:
		float m_Time = .0f;
	};
}