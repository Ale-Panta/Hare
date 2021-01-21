#pragma once

#include "Hare/Renderer/OrthographicCamera.h"
#include "Hare/Core/Timestep.h"
#include "Hare/Events/ApplicationEvent.h"
#include "Hare/Events/MouseEvent.h"

namespace Hare
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool enableRotation = false);

	public:
		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);
		//inline void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
		//inline float GetZoomLevel() const { return m_ZoomLevel; }
		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		glm::vec3 m_CameraPosition = glm::vec3(0.0f);
		float m_CameraTranslationSpeed = 4.0f;

		bool m_RotationEnabled = false;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 180.0f;

		OrthographicCamera m_Camera;
	};
}