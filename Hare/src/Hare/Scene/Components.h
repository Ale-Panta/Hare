#pragma once

#include "Hare/Renderer/Camera.h"
#include "Hare/Scene/SceneCamera.h"
#include "Hare/Scene/ScriptableEntity.h"

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
		TransformComponent(const glm::mat4 & transfrom) : Transform(transfrom) { }

		glm::mat4 Transform = glm::mat4(1.0f);

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}

		glm::vec4 Color = glm::vec4(1.0f);
	};

	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		SceneCamera Camera;
		bool Primary = true;	// #TODO: think about move to scene.
		bool FixedAspectRatio = false;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}