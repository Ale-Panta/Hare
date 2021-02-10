#pragma once
#include "entt.hpp"
#include "Hare/Core/TimeStep.h"

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
		/// <summary>
		/// Create a new entity with default 
		/// transform component and tag component.
		/// </summary>
		Entity CreateEntity(const std::string& name = std::string());

		/// <summary>
		/// Destroy the entity from the registry.
		/// </summary>
		void DestroyEntity(Entity entity);
		void OnUpdate(TimeStep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		template<typename T>	// Template implementation in .cpp file...
		void OnComponentAdded(Entity entity, T& component);

	private:

		/*
		Contains the actual component data.
		It's a container of Entities, and entity
		is just an uint id that contains components.
		*/
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		// Resize our non FixedAspectratio camera.
	};
}