#pragma once

#include <string>
#include <unordered_map>

namespace Hare
{
	class Shader 
	{
	public:
		virtual ~Shader() = default;

	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filePath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
	};

	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);	// Pass it as const reference to prevent incrementing ref counter.
		void Add(const std::string& name, const Ref<Shader>& shader);	// Pass it as const reference to prevent incrementing ref counter.
		Ref<Shader> Load(const std::string& filePath);
		Ref<Shader> Load(const std::string& name, const std::string& filePath);
		Ref<Shader> Get(const std::string& name);
		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;	// Ref count increment when access to unordererd_map.
	};
}