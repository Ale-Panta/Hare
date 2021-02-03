#include "hrpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Hare/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

using namespace glm;

namespace Hare
{
	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE
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
			auto&[transfromRef, spriteRef] = group.get<TransformComponent, SpriteComponent>(entity);
		}
#endif
	}

	Scene::~Scene()
	{

	}


	Entity Scene::CreateEntity(const std::string& name)
	{
		// All entities we are gonna create have by default a transform component.
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>(name);
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(TimeStep ts)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* mainCameraTransform = nullptr;
		{
			auto group = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto& [transformRef, cameraRef] = group.get<TransformComponent, CameraComponent>(entity);

				if (cameraRef.Primary)
				{
					mainCamera = &cameraRef.Camera;
					mainCameraTransform = &transformRef.Transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *mainCameraTransform);

			// Search multiple component inside the registry.
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				// Get the reference to transfrom component
				auto& [transfromRef, spriteRef] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transfromRef, spriteRef.Color);
			}

			Renderer2D::EndScene();
		}
	}

}