#pragma once
#include "entt.hpp"
#include "Hare/Core/TimeStep.h"


namespace Hare
{
	class Entity;

	class Scene
	{
		friend class Entity;

	public:
		Scene();
		~Scene();

	public:
		Entity CreateEntity(const std::string& name = std::string());
		void OnUpdate(TimeStep ts);

		// Temp
		inline entt::registry& GetRegistry() { return m_Registry; }

	private:

		/*
		Contains the actual component data.
		It's a container of Entities, and entity
		is just an uint id that contains components.
		*/
		entt::registry m_Registry;
	};
}