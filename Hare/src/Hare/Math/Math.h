#pragma once

#include <glm/glm.hpp>

namespace Hare::Math
{
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outOrientation, glm::vec3& outScale);
}