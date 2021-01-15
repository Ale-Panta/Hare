#pragma once

#include <string>

namespace Hare
{
	class Shader 
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind();
		void Unbind();

		// --- From here all types of Set function are declared...


	private:
		/*int GetUniformLocation(const std::string& name);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		unsigned int CompileShader(unsigned int type, const std::string& source);*/

		/*
		
		*/

	private:
		// Allow OpenGL to identify obejcts.
		uint32_t m_RendererID;
	};
}