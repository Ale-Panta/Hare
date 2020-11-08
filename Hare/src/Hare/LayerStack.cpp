#include "hrpch.h"
#include "Hare/LayerStack.h"

namespace Hare
{
	LayerStack::LayerStack() 
	{
		m_LayerInsert = m_Layers.begin();
	}

	LayerStack::~LayerStack()
	{
		// For now the layers are allocated only when the application is 
		// shut down.
		// In the future, for example, if we change level we need to recreate
		// the stack.

		for (Layer* layer : m_Layers)
			delete layer;
	}

	// -----------------------------------------------------------------
	// PUSH
	// -----------------------------------------------------------------

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	// -----------------------------------------------------------------
	// POP
	// -----------------------------------------------------------------

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		
		if (it != m_Layers.end()) 
		{
			m_Layers.erase(it);
			m_LayerInsert--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);

		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
		}
	}
}