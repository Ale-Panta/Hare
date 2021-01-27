#include "hrpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

namespace Hare
{
	OpenGLShader::OpenGLShader(const string& filePath)
	{
		HR_PROFILE_FUNCTION();

		string source = ReadFile(filePath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath...

		/*
		Retrieve the name after the last slash, cut also the file extension
		for example if we have: assets/shaders/Texture.glsl,
		we retrieve "Texture".
		*/
		auto lastSlash = filePath.find_last_of("/\\");
		/*
		We handle also the case were the file is positioned in the follow example
		Texture.glsl. So it's not inside to any folder.
		*/
		lastSlash = lastSlash == string::npos ? 0 : lastSlash + 1;

		/*
		Retrieve the name before the dot and after the slash. We handle 
		the case were the extension do not exist.
		*/
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const string& name, const string& vertexSource, const string& fragmentSource)
		: m_Name(name)
	{
		HR_PROFILE_FUNCTION();

		unordered_map<GLenum, string> sources;
		sources[GL_VERTEX_SHADER] = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		HR_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	string OpenGLShader::ReadFile(const string& filePath)
	{
		HR_PROFILE_FUNCTION();

		string result;
		ifstream in(filePath, ios::in | ios::binary);

		if (in)
		{
			in.seekg(0, ios::end);			// What the file size is.
			size_t size = in.tellg();

			if (size != -1)
			{
				result.resize(size);			// Tell us were the actual pointer is.
				in.seekg(0, ios::beg);		// Bring us back to the start.
				in.read(&result[0], size);		// And read the file.
				in.close();
			}
			else
			{
				HR_CORE_ASSERT("Could not read from file {0}", filePath);
			}
		}
		else
		{
			HR_CORE_ERROR("Could not open file '{0}'", filePath);
		}

		return result;
	}

	unordered_map<GLenum, string> OpenGLShader::PreProcess(const string& source)
	{
		HR_PROFILE_FUNCTION();

		unordered_map<GLenum, string> shaderSources;

		const char* typeToken = "#type";			// After this token, the next one tell us what type the shader is.
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);		// Look for the type token through the shader source code.

		while (pos != string::npos)	// Loop until no tokens remain.
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HR_CORE_ASSERT(eol != string::npos, "Syntax error");

			size_t begin = pos + typeTokenLength + 1;	// vertex or fragment token must be spaced by one from #type token.
			string type = source.substr(begin, eol - begin);
			HR_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specification!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);

			// Get the substring from the token to the end of file or until the next token is red.
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	GLenum OpenGLShader::ShaderTypeFromString(const string& type)
	{
		if (type == "vertex")							return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		HR_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	void OpenGLShader::Compile(unordered_map<GLenum, string>& shaderSources)
	{
		HR_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();

		HR_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
		array<GLenum, 2> glShadersIDs;

		int glShaderIDIndex = 0;

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const string& source = kv.second;

			// Create an empty shader handle based on type
			GLuint shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that string's .c_str is NULL character terminated.
			const GLchar* sourceCstr = source.c_str();
			glShaderSource(shader, 1, &sourceCstr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				HR_CORE_ERROR("{0}", infoLog.data());
				HR_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShadersIDs[glShaderIDIndex++] = shader;
		}

		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			// Don't leak shaders either.
			for (auto id : glShadersIDs)
				glDeleteShader(id);

			HR_CORE_ERROR("{0}", infoLog.data());
			HR_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShadersIDs)
			glDetachShader(program, id);

	}

	void OpenGLShader::Bind() const
	{
		HR_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		HR_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const string& name, int value)
	{
		HR_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetFloat(const string& name, float value)
	{
		HR_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat3(const string& name, const glm::vec3& value)
	{
		HR_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const string& name, const glm::vec4& value)
	{
		HR_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const string& name, const glm::mat4& value)
	{
		HR_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const string& name, const int& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const string & name, const float & value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const string & name, const glm::vec2 & value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const string & name, const glm::vec3 & value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const string & name, const glm::vec4 & value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const string & name, const glm::mat3 & matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}