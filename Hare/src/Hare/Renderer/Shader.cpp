#include "hrpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

using namespace std;

namespace Hare
{
	Ref<Shader> Shader::Create(const string& filePath)
	{
		switch (Renderer::GetAPI())
		{
			// #if HR_PLATFORM_WINDOWS
		case RendererAPI::API::None:	HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported");  return nullptr;
			// #elseif ...
		case RendererAPI::API::OpenGL:	return make_shared<OpenGLShader>(filePath);
			// #endif
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const string& name, const string& vertexSource, const string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
			// #if HR_PLATFORM_WINDOWS
		case RendererAPI::API::None:	HR_CORE_ASSERT(false, "RendererAPI::None is currently not supported");  return nullptr;
			// #elseif ...
		case RendererAPI::API::OpenGL:	return make_shared <OpenGLShader>(name, vertexSource, fragmentSource);
			// #endif
		}

		HR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	void ShaderLibrary::Add(const string& name, const Ref<Shader>& shader)
	{
		HR_CORE_ASSERT(!Exists(name), "Shader already exist!");	// if the .find(name) == .end() means we do not find the shader.
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const string& name, const string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const string& name)
	{
		HR_CORE_ASSERT(Exists(name), "Shader not found!");	// if .find() != .end() means we do not find the shader.
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}