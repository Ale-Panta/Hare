#pragma once

#include "Layer.h"
#include "Hare/Core/Core.h"

#include <vector>

namespace Hare
{
	/// <summary>
	/// Wrapper class that layers the event system.
	/// Input are blocked from end to begin layers.
	/// <para>
	/// Usually UI elements need to be place on the latest layer,
	/// because if encautered any inputs it will be the first to blocks it.
	/// </para>
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
		unsigned int m_LayerInsertIndex = 0;
	};
}