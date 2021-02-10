#pragma once

#include "Hare/Core/Base.h"
#include "Hare/Events/Event.h"
#include "Hare/Core/TimeStep.h"

namespace Hare 
{
	class Layer 
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