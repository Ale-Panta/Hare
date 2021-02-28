#pragma once
#include "Hare/Core/Base.h"
#include "Hare/Scene/Scene.h"
#include "Hare/Scene/Entity.h"

namespace Hare
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

	public:
		void SetContext(const Ref<Scene>& context);
		void OnImGuiRender();

		void SetSelectedEntity(Entity entity);
		void AddSelectedEntity(Entity entity);
		inline void ClearSelectionContexts() { m_SelectionContexts.clear(); }
		inline Entity GetSelectedEntity() const { return m_SelectionContext; }
		inline std::vector<Entity> GetSelectedEntities() const { return m_SelectionContexts; }

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
		std::vector<Entity> m_SelectionContexts;
	};
}