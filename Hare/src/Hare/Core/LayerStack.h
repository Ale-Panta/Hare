#pragma once

#include "Layer.h"
#include "Hare/Core/Base.h"

#include <vector>

namespace Hare
{
	// Wrapper class that layers the event system.
	// Input are blocked from end to begin layers.
	// Usually UI elements need to be place on the latest layer,
	// because if encautered any inputs it will be the first to blocks it.
	class LayerStack 
	{
	public:
		LayerStack() = default;
		~LayerStack();

		// Push layer in the first half of list.
		void PushLayer(Layer* layer);
		
		// Push layer at the very last of the list.
		void PushOverlay(Layer* overlay);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end() const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}