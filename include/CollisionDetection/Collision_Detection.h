#pragma once

#include <vector>
#include "Rendering/VertexFormat.h"

bool intersectTriangles(const std::vector<Rendering::VertexFormat>& vertsA, const glm::mat4& modelA, const std::vector<Rendering::VertexFormat>& vertsB, const glm::mat4& modelB);
float intersectRayTriangles(const glm::vec4& ray_pos, const glm::vec4& ray_dir, const glm::mat4& modelMat, const std::vector<Rendering::VertexFormat>& verts);
