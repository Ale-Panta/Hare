#pragma once

#include "Hare/Renderer/Camera.h"

namespace Hare
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

		inline float GetOrthographicSize() const { return m_OrthographicSize; }
		inline void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar	= 1.0f;
		float m_AspectRatio = 0.0f;
	};
}