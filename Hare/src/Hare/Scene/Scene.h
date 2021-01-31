#pragma once
#include "Hare/Core/TimeStep.h"
#include "entt.hpp"

namespace Hare
{
	class Scene
	{
	public:
		Scene();
		~Scene();

	public:
		entt::entity CreateEntity();
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