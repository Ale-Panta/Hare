#pragma once

#include "Hare/Renderer/Shader.h"

#include <glm/glm.hpp>

// TODO : remove
typedef unsigned int GLenum;

namespace Hare
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;

		// High level concepts. Those set methods will change once we have a material system.
		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		inline virtual const std::string& GetName() const override { return m_Name; }

		// --- Int ------------------------------------------------------
		void UploadUniformInt(const std::string& name, const int& value);
		// --------------------------------------------------------------

		// --- Floats -----------------------------------------------------------
		void UploadUniformFloat(const std::string& name, const float& value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
		// -----------------------------------------------------------------------

		// --- Matrix -----------------------------------------------------------
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		// ----------------------------------------------------------------------


	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		static GLenum ShaderTypeFromString(const std::string& type);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		// Allow OpenGL to identify objects.
		uint32_t m_RendererID;
		std::string m_Name;
	};
}