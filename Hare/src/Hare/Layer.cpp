#include "hrpch.h"
#include "Layer.h"

namespace Hare 
{
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName) { }

	Layer::~Layer() { }
}