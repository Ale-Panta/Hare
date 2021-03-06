#pragma once

#include "Hare/Core/TimeStep.h"
#include "Hare/Events/MouseEvent.h"
#include "Hare/Events/ApplicationEvent.h"
#include "Hare/Renderer/OrthographicCamera.h"

namespace Hare
{
	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool enableRotation = false);

	public:
		void OnUpdate(DeltaTime dt);
		void OnEvent(Event& e);
		void OnResize(float width, float height);
		//inline void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
		//inline float GetZoomLevel() const { return m_ZoomLevel; }
		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }
		inline const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		OrthographicCameraBounds m_Bounds;

		glm::vec3 m_CameraPosition = glm::vec3(0.0f);
		float m_CameraTranslationSpeed = 4.0f;

		bool m_RotationEnabled = false;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 180.0f;

		OrthographicCamera m_Camera;
	};
}