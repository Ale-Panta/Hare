#pragma once
#include "Hare/Core/Core.h"
#include "Hare/Core/Log.h"
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

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}