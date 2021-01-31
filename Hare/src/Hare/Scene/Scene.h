#pragma once
#include "entt.hpp"

namespace Hare
{
	class Scene
	{
	public:
		Scene();
		~Scene();

	private:
		/*
		Contains the actual component data.
		It's a container of Entities, and entity
		is just an uint id that contains components.
		*/
		entt::registry m_Registry;
	};
}