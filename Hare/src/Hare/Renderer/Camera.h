#pragma once

#include <glm/glm.hpp>

namespace Hare
{
	class Camera
	{
	public:
		Camera(const glm::mat4& projectionMatrix)
			: m_ProjectionMatrix(projectionMatrix){ }

	public:
		inline const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }

	private:
		glm::mat4 m_ProjectionMatrix;
	};
}