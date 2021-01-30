#include "hrpch.h"
#include "Hare/Core/Input.h"
#include "Hare/Core/Keycodes.h"
#include "Hare/Renderer/OrthographicCameraController.h"

namespace Hare
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool enableRotation)
		: m_AspectRatio(aspectRatio),
		m_RotationEnabled(enableRotation),
		m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Bounds(OrthographicCameraBounds{ -aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel })
	{
	}

	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
		HR_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(HR_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;

		else if (Input::IsKeyPressed(HR_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(HR_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

		else if (Input::IsKeyPressed(HR_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;

		if (m_RotationEnabled)
		{
			if (Input::IsKeyPressed(HR_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;

			else if (Input::IsKeyPressed(HR_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
		
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event & e)
	{
		HR_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseScrolledEvent>(HR_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HR_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent & e)
	{
		HR_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);	// Use clamp instead of max to avoid too far situation.
		m_Bounds = OrthographicCameraBounds{ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent & e)
	{
		HR_PROFILE_FUNCTION();

		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeigth();
		m_Bounds = OrthographicCameraBounds{ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);

		return false;
	}
}