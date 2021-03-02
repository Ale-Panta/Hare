#pragma once
#include "entt.hpp"
#include "Hare/Core/TimeStep.h"
#include "Hare/Renderer/EditorCamera.h"

namespace Hare
{
	class Entity;

	class Scene
	{
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;

	public:
		Scene();
		~Scene();

	public:
		/// Create a new entity with default 
		/// ID, Transform and Tag component.
		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithID(const uint64_t uuid, const std::string& name = std::string());

		/// Destroy the entity from the registry.
		void DestroyEntity(Entity entity);
		void OnUpdateRuntime(DeltaTime dt);
		void OnUpdateEditor(DeltaTime dt, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();

	private:
		template<typename T>	// Template implementation in .cpp file...
		void OnComponentAdded(Entity entity, T& component);

	private:

		// Contains the actual component data.
		// It's a container of Entities, and entity
		// is just an uint id that contains components.
		entt::registry m_Registry;

		using EntityMap = std::map<uint64_t, Entity>;
		EntityMap m_EntityMapID;

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		// Resize our non FixedAspectratio camera.
	};
}