#pragma once

namespace Hare
{
	// Context Interface. It will be implemented for every platform.

	class GraphicsContext 
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Scope<GraphicsContext> Create(void* window);
	};
}