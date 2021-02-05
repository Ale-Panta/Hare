//#include "hrpch.h"
#include "SceneHierarchyPanel.h"
#include "Hare/Scene/Components.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Hare
{

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		// Hierarchy...
		ImGui::Begin("Scene Hierarchy");
		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity = Entity(entityID, m_Context.get());
				DrawEntityNode(entity);
			});
		ImGui::End();

		// Deselection...
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// Components...
		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				const char* projectionTypeString[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeString[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeString[i];
						if (ImGui::Selectable(projectionTypeString[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeString[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float orthoFOV = glm::degrees(camera.GetPerspectiveVericalFOV());
					if (ImGui::DragFloat("FOV", &orthoFOV))
						camera.SetPerspectiveVericalFOV(glm::radians(orthoFOV));

					float perspectiveNearClip = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &perspectiveNearClip))
						camera.SetPerspectiveNearClip(perspectiveNearClip);

					float perspectiveFarClip = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &perspectiveFarClip))
						camera.SetPerspectiveFarClip(perspectiveFarClip);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNearClip = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &orthoNearClip))
						camera.SetOrthographicNearClip(orthoNearClip);

					float orthoFarClip = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &orthoFarClip))
						camera.SetOrthographicFarClip(orthoFarClip);

					ImGui::Checkbox("Fixed Aspect ratio", &cameraComponent.FixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}
	}

}