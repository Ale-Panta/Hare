#pragma once

#include "Hare/Core.h"
#include "Layer.h"

#include <vector>

namespace Hare
{
	/// <summary>
	/// Wrapper class that layers the event system.
	/// Input are blocked from end to begin layers.
	/// <example>
	/// Usually UI elements need to be place on the latest layer,
	/// because if encautered any inputs it will be the first to blocks it.
	/// </example>
	/// </summary>
	class HARE_API LayerStack 
	{
	public:
		LayerStack();
		~LayerStack();

		/// <summary>
		/// Push layer in the first half of list.
		/// </summary>
		void PushLayer(Layer* layer);
		
		/// <summary>
		/// Push layer at the very last of the list.
		/// </summary>
		void PushOverlay(Layer* overlay);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};
}