#pragma once

namespace Hare
{
	enum class RendererAPI
	{
		None = 0,
		OpenGL
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
		inline static void SetAPI(RendererAPI api) { s_RendererAPI = api; }	// Define it before create the application context.

	private:
		static RendererAPI s_RendererAPI;
	};
}