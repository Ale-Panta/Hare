#pragma once

#include "Hare/Core/Layer.h"
#include "Hare/Events/KeyEvent.h"
#include "Hare/Events/MouseEvent.h"
#include "Hare/Events/ApplicationEvent.h"

namespace Hare
{
	class ImGuiLayer : public Layer 
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();	// Begin ImGui rendering
		void End();		// End ImGui rendering

		inline void SetBLockEvents(bool block) { m_BlockEvents = block; }

		void SetDarkThemeColor();

	private:
		bool m_BlockEvents = true;
		float m_Time = .0f;
	};
}