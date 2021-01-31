#include "hrpch.h"
#include "Scene.h"

#include <glm/glm.hpp>

using namespace glm;

namespace Hare
{
	Scene::Scene()
	{
		// Below is a demonstration of how to create a component
		struct TransformComponent
		{
			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const mat4& transfrom)
				: Transfrom(transfrom) { }


			mat4 Transfrom;

			operator mat4&() { return Transfrom; }
			operator const mat4&() const { return Transfrom; }
		};

		struct SpriteComponent {};

		TransformComponent transform;

		// Create an entity
		entt::entity entity = m_Registry.create();

		// We attach the transfrom component to entity.
		m_Registry.emplace<TransformComponent>(entity, transform);

		// Check if a component live belongs to an entity
		if (m_Registry.has<TransformComponent>(entity))
		{
			// Get the reference to transfrom component
			TransformComponent& transformRef = m_Registry.get<TransformComponent>(entity);
		}

		// Remove the transform component from entity.
		m_Registry.remove<TransformComponent>(entity);

		// Search a specific component
		auto view = m_Registry.view<TransformComponent>();
		for (auto ent : view)
		{
			// Get the reference to transfrom component
			TransformComponent& transformRef = view.get<TransformComponent>(entity);
		}

		// Search multiple component inside the registry.
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
		for (auto ent : group)
		{
			// Get the reference to transfrom component
			group.get<TransformComponent, SpriteComponent>(entity);
		}
	}

	Scene::~Scene()
	{

	}
}