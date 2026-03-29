#pragma once
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace object3D
{
	Mesh* CreateMountain(const std::string& name, glm::vec3 color);
	Mesh* CreateCylinder(
		const std::string& name,
		glm::vec3 color,
		float radius,
		float height,
		int segments);

	Mesh* CreateTriangle(
		const std::string& name,
		glm::vec3 leftBottomCorner,
		float length,
		glm::vec3 color,
		bool fill);

	Mesh* CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, bool fill, bool half);
}