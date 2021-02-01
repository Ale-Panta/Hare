#pragma once

#include <glm/glm.hpp>

namespace Hare
{
	/*
	Every component must be a struct.
	It must provides constructor, copy constructor
	and eventually move constructor.
	It must also provide at least one member variable.
	Operator overloads are only syntactic sugar.
	*/

	struct TagComponent
	{
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) { }

		std::string Tag;
	};

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4 & transfrom) : Transfrom(transfrom) { }


		glm::mat4 Transfrom = glm::mat4(1.0f);

		operator glm::mat4& () { return Transfrom; }
		operator const glm::mat4& () const { return Transfrom; }
	};

	struct SpriteRendererComponent
	{
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}


		glm::vec4 Color = glm::vec4(1.0f);
	};
}