#include "hrpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Hare/Core/Definitions.h"
#include "Hare/Renderer/Renderer2D.h"
//#include "Hare/Renderer/EditorCamera.h"


#include <glm/glm.hpp>
#include <algorithm>
#include <glad/glad.h>

using namespace glm;
using namespace std;

namespace Hare
{
	struct DrawElements
	{
		TransformComponent Transform;
		SpriteRendererComponent Sprite;
		int RedID;
	};

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		// All entities we are gonna create have by default a transform component.
		Entity entity = Entity(m_Registry.create(), this);

		auto& idComponent = entity.AddComponent<IDComponent>();
		idComponent.ID = IDComponent().ID;

		entity.AddComponent<TransformComponent>();

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_EntityMapID[idComponent.ID] = entity;

		return entity;
	}


	Hare::Entity Scene::CreateEntityWithID(const uint64_t uuid, const std::string& name /*= std::string()*/)
	{
		// All entities we are gonna create have by default a transform component.
		Entity entity = Entity(m_Registry.create(), this);

		auto& idComponent = entity.AddComponent<IDComponent>(uuid);

		entity.AddComponent<TransformComponent>();

		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		HR_CORE_ASSERT(m_EntityMapID.find(idComponent.ID) == m_EntityMapID.end());
		m_EntityMapID[idComponent.ID] = entity;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateRuntime(DeltaTime dt)
	{
		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					// #TODO: Move to Scene::OnScenePlay.
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity(entity, this);
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(dt);
				});
		}

		// Render
		Camera* mainCamera = nullptr;
		glm::mat4 mainCameraTransform;
		glm::vec3 mainCameraPosition;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transformRef, cameraRef] = view.get<TransformComponent, CameraComponent>(entity);

				if (cameraRef.Primary)
				{
					mainCamera = &cameraRef.Camera;
					mainCameraTransform = transformRef.GetTransform();
					mainCameraPosition = transformRef.Translation;
					break;
				}
			}
		}

		if (mainCamera)
		{
			// We need to render the object following those steps:
			// * Draw all opaque object first.
			// * Sort all transparent objects.
			// * Draw all transparent objects in order.

			Renderer2D::BeginScene(*mainCamera, mainCameraTransform);

			// I choose to use a map to order the transparency because it order automatically the keys when assigned.
			map<float, DrawElements> sorted;

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				// Check if it has a texture. If it is so add it to the map 
				if (sprite.ToggleTexture)
				{
					float distance = glm::length(mainCameraPosition - transform.Translation);
					sorted[distance] = { transform, sprite, (int)entity };
				}
				// Render it normally
				else
				{
					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				}
			}

			for (auto it = sorted.rbegin(); it != sorted.rend(); ++it)
			{
				Renderer2D::DrawSprite(it->second.Transform.GetTransform(), it->second.Sprite, it->second.RedID);
			}

			Renderer2D::EndScene();
		}
	}


	void Scene::OnUpdateEditor(DeltaTime dt, EditorCamera& camera)
	{
		// We need to render the object following those steps:
		// * Draw all opaque object first.
		// * Sort all transparent objects.
		// * Draw all transparent objects in order.

		Renderer2D::BeginScene(camera);

		// I choose to use a map to order the transparency because it order automatically the keys when assigned.
		map<float, DrawElements> sorted;

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			// Check if it has a texture. If it is so add it to the map 
			if (sprite.ToggleTexture)
			{
				float distance = glm::length(camera.GetPosition() - transform.Translation);
				sorted[distance] = { transform, sprite, (int)entity };
			}
			// Render it normally
			else
			{
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}
		}

		for (auto it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			Renderer2D::DrawSprite(it->second.Transform.GetTransform(), it->second.Sprite, it->second.RedID);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);

			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}


	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity(entity, this);
		}
		return {};	// Return empty entity.
	}

	/*
	* #TEMP specialized template... in the future we will have a table or dictionary
	* where to store the component ID and trigger ComponentAdded().
	*/

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		//TODO let this work on linux
#ifdef HR_PLATFORM_WINDOWS
		static_assert(false);
#endif
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
}