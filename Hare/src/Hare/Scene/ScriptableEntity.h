#pragma once
#include "Entity.h"

namespace Hare
{
	class ScriptableEntity : public Entity
	{
		friend class Scene;

	public:
		virtual ~ScriptableEntity() { }

	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() { }
		virtual void OnDestroy() { }
		virtual void OnUpdate(DeltaTime dt) { }

	private:
		Entity m_Entity;

	};
}