#pragma once

#include "Hare/Core/Core.h"
#include "Hare/Events/Event.h"
#include "Hare/Core/TimeStep.h"

namespace Hare 
{
	class HARE_API Layer 
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() { }
		virtual void OnDetach() { }
		virtual void OnUpdate(TimeStep ts) { }
		virtual void OnImGuiRender() { }
		virtual void OnEvent(Event& e) { }

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}