#pragma once

#include "glm\glm.hpp"

namespace Rendering {

	struct Rect {

		//Current Position
		glm::vec2 top_left;
		glm::vec2 bottom_right;
		//Original Position
		glm::vec2 orig_top_left;
		glm::vec2 orig_bottom_right;

		Rect(const glm::vec2 &tl, const glm::vec2 &br) {
			orig_top_left = tl;
			orig_bottom_right = br;
		}

		//Standard Constructor
		Rect() {}

		void UpdateRect(glm::mat4& mat) {
			glm::vec4 tl = mat * glm::vec4(orig_top_left.x, orig_top_left.y, 0.0f, 1.0f);
			glm::vec4 br = mat * glm::vec4(orig_bottom_right.x, orig_bottom_right.y, 0.0f, 1.0f);
			top_left = glm::vec2(tl.x, tl.y);
			bottom_right = glm::vec2(br.x, br.y);
		}
	};
}
