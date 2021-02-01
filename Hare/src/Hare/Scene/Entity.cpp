#include "hrpch.h"
#include "Entity.h"

namespace Hare
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle),
		m_Scene(scene)
	{

	}
}