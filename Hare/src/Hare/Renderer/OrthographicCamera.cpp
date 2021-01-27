#include "hrpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace Hare
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		HR_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		HR_PROFILE_FUNCTION();

		m_ProjectionMatrix = ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		HR_PROFILE_FUNCTION();

		mat4 transfrom = 
			translate(mat4(1.0f), m_Position) * 
			rotate(mat4(1.0f), radians(m_Rotation), vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = inverse(transfrom);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}