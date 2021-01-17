#pragma once

#include "Hare/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Hare
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		// --- From here all types of Set function are declared...

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
		// Allow OpenGL to identify obejcts.
		uint32_t m_RendererID;
	};
}