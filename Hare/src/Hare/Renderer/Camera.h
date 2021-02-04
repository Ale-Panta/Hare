#pragma once

#include <glm/glm.hpp>

namespace Hare
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projectionMatrix)
			: m_ProjectionMatrix(projectionMatrix){ }
		virtual ~Camera() = default;

	public:
		inline const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }

	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	};
}