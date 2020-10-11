#pragma once

#include "glm/glm.hpp"

namespace Rendering {

	struct VertexFormat {

		glm::vec4 position;
		glm::vec4 color;
		glm::vec2 texcoord;

		VertexFormat(const glm::vec4 &pos, const glm::vec4 &col, const glm::vec2 &tex) {
			position = pos;
			color = col;
			texcoord = tex;
		}

		VertexFormat(const glm::vec4& pos, const glm::vec2& tex) {
			position = pos;
			texcoord = tex;
			color = glm::vec4(-1.0f, -1.0f, -1.0f, -1.0f); //DEFAULT
		}

		VertexFormat(const glm::vec4& pos, const glm::vec4& col) {
			position = pos;
			color = col;
			texcoord = glm::vec2(-1.0f, -1.0f); //DEFAULT
		}

		VertexFormat(const glm::vec4 &pos) {
			position = pos;
			color = glm::vec4(-1.0f, -1.0f, -1.0f, -1.0f); //DEFAULT
			texcoord = glm::vec2(-1.0f, -1.0f); //DEFAULT
		}

		//Standard Constructor
		VertexFormat() {}
	};
}
